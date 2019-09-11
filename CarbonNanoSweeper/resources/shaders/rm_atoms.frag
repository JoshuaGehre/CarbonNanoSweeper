#version 430 core

const float pi = 3.14159265358979323846264338;

const float radius = sin(pi / 3) / sin(pi / 16);

const float rSin16 = radius * sin(pi / 16);

const float rCos16 = radius * cos(pi / 16);

const float rSin8 = radius * sin(pi / 8);

const float rCos8 = radius * cos(pi / 8);

const float rSin4 = radius * sin(pi / 4);

const float rCos4 = radius * cos(pi / 4);

const int mirrorCount = 7;

const vec3 mirrors[7][2] = 
{
  {vec3(0.75, -0.5 * rSin16, 0.5 * rCos16 + 0.5 * radius), vec3(-0.5, rSin16, radius - rCos16)},
  {vec3(0, -rSin16, rCos16), vec3(0, cos(pi/16), sin(pi/16))},
  {vec3(0, -rSin8, rCos8), vec3(0, cos(pi/8), sin(pi/8))},
  {vec3(0, -rSin4, rCos4), vec3(0, cos(pi/4), sin(pi/4))},
  {vec3(0, -radius, 0), vec3(0, 0, 1)},
  {vec3(0, 0, 0), vec3(0, -1, 0)},
  {vec3(0, 0, 0), vec3(1, 0, 0)}
};

in vec2 v_texcoord;

layout(location = 0) out vec4 frag_color;

layout(location = 1) out vec4 frag_depth;

layout(std140,binding = 1) uniform vectorBlock
{
  vec4 Position;
	vec4 Direction;
	vec4 Up;
	vec4 Right;
} camVectors;

layout(std140,binding = 4) uniform colorBlock
{
  vec4 Background;
	vec4 Atoms;
	vec4 Bonds;
} Colors;

float distanceEstimator(vec3 p)
{
  p.x += 1.5;
  p.x -= 3 * floor(p.x / 3);
  p.x -= 1.5;
  for(int j = mirrorCount - 1; j >= 0; j--){
    vec3 md = p - mirrors[j][0];
    float l = dot(md, mirrors[j][1]);
    if(l < 0){
      p -= 2 * l * mirrors[j][1];
    }
  }
  return max(0, length(p - vec3(0.5, 0, radius)) - 0.25);
}

vec3 getClosestCenter(vec3 pIn){
  vec3 p = pIn;
  p.x += 1.5;
  p.x -= 3 * floor(p.x / 3);
  p.x -= 1.5;
  int refl = 0;
  for(int j = mirrorCount - 1; j >= 0; j--){
    vec3 md = p - mirrors[j][0];
    float l = dot(md, mirrors[j][1]);
    refl <<= 1;
    if(l < 0){
      p -= 2 * l * mirrors[j][1];
      refl += 1;
    }
  }
  
  vec3 c = vec3(0.5, 0, radius);
  
  for(int j = 0; j < mirrorCount; j++){
    if((refl & 1) == 1){
      vec3 md = c - mirrors[j][0];
      c -= 2 * mirrors[j][1] * dot(mirrors[j][1], md);
    }
    refl >>= 1;
  }
  
  c.x += 3 * floor((pIn.x + 1.5) / 3);
    
  return c;
}

void main()
{
  // Ray Direction
  vec3 pos = camVectors.Position.xyz;
  vec3 dir = normalize(camVectors.Direction.xyz
    + (2 * v_texcoord.x - 1) * camVectors.Right.xyz
    + (2 * v_texcoord.y - 1) * camVectors.Up.xyz);
    
  for(int i = 0; i < 64; i++){
    float dist = distanceEstimator(pos);
    pos += dir * dist;
    if(dist > 10000) discard;
  }

  if(distanceEstimator(pos) < 0.5){
    float d = max(0, min(1, 1.5 - length(pos - camVectors.Position.xyz) / 30));
    vec3 center = getClosestCenter(pos);
    vec3 nrm = normalize(pos - center);
    float b = 0.25 + 0.75 * max(0, dot(-dir, nrm));
    frag_color = vec4(d * b * Colors.Atoms + (1 - d) * Colors.Background);
    frag_depth = vec4(1.0 / length(pos - camVectors.Position.xyz), 0, 0, 0);
  }else{
    discard;
  }
}
