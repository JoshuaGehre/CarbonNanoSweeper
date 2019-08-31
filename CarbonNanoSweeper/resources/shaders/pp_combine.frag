#version 430 core

in vec2 v_texcoord;

layout(location = 0) out vec4 frag_color;

// Atoms

layout(binding = 0) uniform sampler2D texAtoms;

layout(binding = 1) uniform sampler2D depthAtoms;

// Bonds

layout(binding = 2) uniform sampler2D texBonds;

layout(binding = 3) uniform sampler2D depthBonds;

// Cylinder

layout(binding = 4) uniform sampler2D texCylinder;

layout(binding = 5) uniform sampler2D texCylinderBack;

layout(binding = 6) uniform sampler2D depthCylinder;

layout(std140,binding = 4) uniform colorBlock
{
  vec4 Background;
	vec4 Atoms;
	vec4 Bonds;
} Colors;

void main()
{
  float dAtoms = texture(depthAtoms, v_texcoord).x;
  float dBonds = texture(depthBonds, v_texcoord).x;
  float dStruct = max(dAtoms, dBonds);
  vec4 colStruct = ((dAtoms > dBonds) ? texture(texAtoms, v_texcoord) : texture(texBonds, v_texcoord));
  
  vec2 dCylFB = texture(depthCylinder, v_texcoord).xy;
  float dCylinderF = dCylFB.x;
  float dCylinderB = dCylFB.y;
  
  vec3 col = Colors.Background.xyz;
  
  if(dStruct > 0) col = colStruct.xyz;

  if(dCylinderB > 0){
    vec4 colCylB = texture(texCylinderBack, v_texcoord); 
    col = col * (1 - colCylB.w) + colCylB.xyz * colCylB.w;
  }
  if((dCylinderF > dStruct) && (dStruct > dCylinderB)){
    col = colStruct.xyz;
  }
  if(dCylinderF > 0){
    vec4 colCylF = texture(texCylinder, v_texcoord); 
    col = col * (1 - colCylF.w) + colCylF.xyz * colCylF.w;
  }
  if(dStruct > dCylinderF){
    col = colStruct.xyz;
  }
  frag_color = vec4(col, 1);
}
