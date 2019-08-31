#version 430 core

const int repeat = 16 * 24;

const float pi = 3.14159265358979323846264338;

const float radius = sin(pi / 3) / sin(pi / 16);

in vec2 v_texcoord;

layout(binding = 0) uniform sampler2D tiles;

layout(location = 0) out vec4 frag_color;

layout(location = 1) out vec4 frag_color_back;

layout(location = 2) out vec4 frag_depth;

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

layout(std140,binding = 2) uniform tileIdStruct
{
    ivec4 t[4 * 24];
} tilesIds;

layout(std140,binding = 3) uniform highlightStruct
{
    ivec4 Hex_TimeMs_E_E;
} highlight;

vec4 sampleColor(vec3 pos){
  float phi0 = (atan(pos.z, pos.y) / pi) * 0.5;
  if(phi0 < 0) phi0 += 1;
  phi0 *= 16;
  float phi = phi0;
  float xSeg = 0.5 + pos.x - 3 * floor((pos.x + 0.5) / 3);
  
  int row = int(floor((pos.x + 0.5)/1.5));
  
  int num = 0;
  
  if(xSeg > 1.5){
    phi += 0.5;
    xSeg -= 1.5;
    if(phi >= 16) phi -= 16;
  }
  if(xSeg < 1){
    num = int(phi);
  }else{
    xSeg = 2 * (xSeg - 1);
    float h = 1 - 2 * abs(phi - floor(phi) - 0.5);
    if(xSeg < h){
      num = int(phi);
    }else{
      num = int(floor(phi + 0.5));
      if((row & 1) == 1) num -= 1;
      row += 1;
      if(num == -1) num = 15;
      if(num == 16) num = 0;
    }
  }
  
  int hexId = row * 16 + num;//% repeat;
  if(hexId > 0){
    hexId %= repeat;
  }else{
    hexId = repeat - 1 - (repeat - 1 - hexId) % repeat;
  }
  // Find center
  
  float centerX = 1.5 * row;
  float centerPhi = num + (((row & 1) == 1) ? 0 : 0.5);
  
  float dx = pos.x - centerX;
  float dPhi = phi0 - centerPhi;
  if(dPhi < -2) dPhi += 16;
  if(dPhi > 2) dPhi -= 16;
  
  int tileId = tilesIds.t[(hexId & 0xffff) >> 2][hexId & 3];
  
  vec4 color = texture(tiles, vec2((tileId + (0.5 + 0.5 * dx)) / 11, 0.5 - dPhi * 0.8660254038));
  if(highlight.Hex_TimeMs_E_E.x == hexId){
    float f = 2 + 0.25 * sin(highlight.Hex_TimeMs_E_E.y / 300.0);
    color = vec4(color.xyz * f, color.w);
  }
  
  return color;
}

void main()
{
  // Ray Direction
  vec3 pos = camVectors.Position.xyz;
  vec3 dir = normalize(camVectors.Direction.xyz
    + (2 * v_texcoord.x - 1) * camVectors.Right.xyz
    + (2 * v_texcoord.y - 1) * camVectors.Up.xyz);
    
  // 2D
  // a * y + b * z = c
  // y * y + z * z = r * r
  float a = -dir.z;
  float b = dir.y;
  float c = pos.y * a + pos.z * b;
  
  bool flipYZ = false;
  
  if(abs(a) < 0.001){
    if(abs(b) < 0.001){
      discard;
    }else{
      flipYZ = true;
      b = -dir.z;
      a = dir.y;
    }
  }
  
  // y = c / a - b / a * z
  
  float d = c / a;
  float e = - b / a;
  
  // y = d + e * z
  // (d + e * z) * (d + e * z) + z * z = r * r
  // d * d + 2 * d * e + e * e * z * z + z * z = r * r 
  // z * z * (1 + e * e) + z * (2 * d * e) + d * d - r * r = 0
  
  float f = 1 + e * e;
  float g = 2 * d * e;
  float h = d * d - radius * radius;
  
  // z * z * f + z * g + h = 0
  
  g /= f;
  h /= f;
  
  // z * z + z * g + h = 0
  
  float i = 0.25 * g * g - h;
  
  if(i < 0) discard;
  
  i = sqrt(i);
  float j = - g * 0.5;
  
  // z1/2 = j +- i
  
  float z1 = j + i;
  float z2 = j - i;
  
  float z0 = flipYZ ? -pos.y : pos.z;
  
  float l1 = (z0 - z1) / a;
  float l2 = (z0 - z2) / a;
  
  if(l1 < 0) discard;
  
  vec3 posFront = pos + dir * min(l1, l2);
  vec3 posBack = pos + dir * max(l1, l2);
  
  vec4 colF = sampleColor(posFront);
  vec4 colB = vec4(0.5, 0.5, 0.5, sampleColor(posBack).w);
  
  frag_depth = vec4(1.0 / length(posFront - camVectors.Position.xyz), 1.0 / length(posBack - camVectors.Position.xyz), 0, 0);
  
  vec3 nrm = normalize(vec3(0, posFront.yz));
  d = max(0, min(1, 1.5 - length(posFront - camVectors.Position.xyz) / 30));
  b = 0.25 + 0.75 * max(0, dot(-dir, nrm));
  frag_color = vec4(d * b * colF.xyz + (1 - d) * Colors.Background.xyz, colF.w);
  
  nrm = normalize(vec3(0, -posBack.yz));
  d = max(0, min(1, 1.5 - length(posBack - camVectors.Position.xyz) / 30));
  b = 0.25 + 0.75 * max(0, dot(-dir, nrm));
  frag_color_back = vec4(d * b * colB.xyz + (1 - d) * Colors.Background.xyz, colB.w);;
  
}
