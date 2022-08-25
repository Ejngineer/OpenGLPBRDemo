#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 WorldPos;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

uniform vec3 camPos;

uniform sampler2D Albedo;
uniform sampler2D normalMap;
uniform sampler2D Metallic;
uniform sampler2D Roughness;
uniform sampler2D AO;

uniform float Divisions;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

const float PI = 3.14159265359;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords * Divisions).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float NDF(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;
	float numerator = a2;
	float denominator = (NdotH2*(a2 - 1.0) + 1.0);
	denominator = PI * (denominator * denominator);

	return numerator / denominator;
};

vec3 Fresnel(float HdotV, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - (HdotV),0.0,1.0), 5.0);
}

vec3 FresnelRoughness(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float GGX_sub(vec3 N, vec3 V, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;
	float NdotV = max(dot(N,V), 0.0);
	float numerator = NdotV;
	float denominator = (NdotV) * (1.0 - k) + k;

	return numerator / denominator;
}

float GGX(vec3 N, vec3 V, vec3 L, float k)
{
	float GGV = GGX_sub(N,V,k);
	float GGL = GGX_sub(N,L,k);
	return GGV * GGL;
}

void main()
{
	vec3 N = getNormalFromMap();
	vec3 V = normalize(camPos - WorldPos);


	vec3 albedo = pow(texture(Albedo, TexCoords * Divisions).rgb, vec3(2.2));
	float metallic = texture(Metallic, TexCoords * Divisions).r;
	float ao = texture(AO, TexCoords * Divisions).r;
	float textroughness = texture(Roughness, TexCoords * Divisions).r;

	float eta = 1.00 / 1.52;
	vec3 R = reflect(-V, N);
	const float MAX_REFLECTION_LOD = 4.0;
	vec3 prefiltercolor = textureLod(prefilterMap, R  * Divisions, textroughness * MAX_REFLECTION_LOD).rgb;

	vec3 L;
	vec3 H;

	float dist = 0.0;
	float attenuation = 0.0;
	vec3 radiance = vec3(0.0);
	
	vec3 F0 = vec3(0.0);;
	vec3 F = vec3(0.0);;
	
	float D = 0.0;
	float G = 0.0;

	vec3 numerator = vec3(0.0);
	float denominator = 0.0;
	vec3 specular = vec3(0.0);

	vec3 Ks = vec3(0.0);
	vec3 Kd = vec3(0.0);
	float NdotL = 0.0;

	F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);
	
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < 1; ++i)
	{
		L = normalize(lightPositions[i] - WorldPos);
		H = normalize(V + L);

		dist = length(lightPositions[i] - WorldPos);
		attenuation = 1.0 / (dist * dist);
		radiance = lightColors[i] * attenuation;

		D = NDF(N, H, textroughness);
		G = GGX(N, V, L, textroughness);
		F = Fresnel(max(dot(H,V),0.0), F0);

		numerator = D * G * F;
		denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N,L) ,0.0) + 0.0001;
		specular = numerator / denominator;

		Ks = F;
		Kd = vec3(1.0) - Ks;

		Kd *= 1.0 - metallic;

		NdotL = max(dot(N,L), 0.0);
		Lo += (Kd * albedo / PI + specular) * radiance * NdotL;
	}

	F = FresnelRoughness(max(dot(N,V), 0.0), F0, textroughness);
	Ks = F;
	Kd = vec3(1.0) - Ks;
	Kd *= 1.0 - metallic;
	vec3 irradiance = texture(irradianceMap, N  * Divisions).rgb;
	vec3 diffuse = irradiance * albedo;

	vec2 envBRDF = texture(brdfLUT, vec2(max(dot(N, V), 0.0) * Divisions, textroughness)).rg;
	specular = prefiltercolor * (F * envBRDF.x + envBRDF.y);

	vec3 ambient = (Kd * diffuse + specular) * ao;

	vec3 color = ambient + Lo;

	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));

	FragColor = vec4(color, 1.0);

}
