#version 330 core

out vec4 frag_color;

in vec2 tex_coord_out;
in vec3 frag_pos_out;
in vec3 normals_out;

struct Light 
{
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
	vec3 specular;
};

uniform Light light;
uniform vec3 view_pos;

struct Material 
{
	sampler2D texturas[32];
	int count;
};

uniform Material material;


void main()
{
	vec3 lightDir = normalize(-light.direction);

	// añadimos cada textura al color final:
	for(int i = 0; i < material.count; ++i) 
	{
		// ambient
		vec3 ambient = light.ambient * texture(material.texturas[i], tex_coord_out).rgb;

		// diffuse
		vec3 norm = normalize(normals_out);
		vec3 light_dir = normalize(-light.direction);  
		float diff = max(dot(norm, light_dir), 0.0);
		vec3 diffuse = light.diffuse * diff * texture(material.texturas[i], tex_coord_out).rgb; 

		// specular
		vec3 view_dir = normalize(view_pos - frag_pos_out);
		vec3 reflect_dir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
		vec3 specular = light.specular * spec * texture(material.texturas[i], tex_coord_out).rgb;  

		vec3 result = ambient + diffuse + specular;

		frag_color += vec4(result, 1.0);
	}
}