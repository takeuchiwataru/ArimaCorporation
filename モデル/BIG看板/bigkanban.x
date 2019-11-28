xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 60;
 -335.14679;122.48184;-28.68111;,
 -335.14671;122.48190;-9.76469;,
 -335.14671;436.40560;-9.76469;,
 -335.14679;436.40572;-28.68111;,
 307.98594;122.48184;-28.68114;,
 -335.14679;122.48184;-28.68111;,
 -335.14679;436.40572;-28.68111;,
 307.98594;436.40572;-28.68114;,
 -335.14679;436.40572;-28.68111;,
 -335.14671;436.40560;-9.76469;,
 307.98586;436.40560;-9.76471;,
 307.98594;436.40572;-28.68114;,
 307.98594;436.40572;-28.68114;,
 307.98586;436.40560;-9.76471;,
 307.98586;122.48190;-9.76471;,
 307.98594;122.48184;-28.68114;,
 307.98594;122.48184;-28.68114;,
 307.98586;122.48190;-9.76471;,
 -335.14671;122.48190;-9.76469;,
 -335.14679;122.48184;-28.68111;,
 -335.14671;122.48190;-9.76469;,
 307.98586;122.48190;-9.76471;,
 307.98586;436.40560;-9.76471;,
 -335.14671;436.40560;-9.76469;,
 248.25480;0.26133;22.82325;,
 254.11266;0.26133;8.68111;,
 254.11266;281.32742;8.68111;,
 248.25480;281.32742;22.82325;,
 248.25480;0.26133;-5.46103;,
 248.25480;281.32742;-5.46103;,
 234.11266;0.26133;-11.31889;,
 234.11266;281.32742;-11.31889;,
 219.97053;0.26133;-5.46103;,
 219.97053;281.32742;-5.46103;,
 214.11266;0.26133;8.68111;,
 214.11266;281.32742;8.68111;,
 219.97053;0.26133;22.82325;,
 219.97053;281.32742;22.82325;,
 234.11266;0.26133;28.68111;,
 234.11266;281.32742;28.68111;,
 248.25480;0.26133;22.82325;,
 248.25480;281.32742;22.82325;,
 -253.45749;0.26133;22.82327;,
 -247.59964;0.26133;8.68113;,
 -247.59964;281.32742;8.68113;,
 -253.45749;281.32742;22.82327;,
 -253.45749;0.26133;-5.46101;,
 -253.45749;281.32742;-5.46101;,
 -267.59964;0.26133;-11.31887;,
 -267.59964;281.32742;-11.31887;,
 -281.74177;0.26133;-5.46101;,
 -281.74177;281.32742;-5.46101;,
 -287.59964;0.26133;8.68113;,
 -287.59964;281.32742;8.68113;,
 -281.74177;0.26133;22.82327;,
 -281.74177;281.32742;22.82327;,
 -267.59964;0.26133;28.68113;,
 -267.59964;281.32742;28.68113;,
 -253.45749;0.26133;22.82327;,
 -253.45749;281.32742;22.82327;;
 
 34;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;25,28,29,26;,
 4;28,30,31,29;,
 4;30,32,33,31;,
 4;32,34,35,33;,
 4;34,36,37,35;,
 4;36,38,39,37;,
 4;38,40,41,39;,
 3;31,33,29;,
 3;33,35,29;,
 3;29,35,26;,
 3;35,37,26;,
 3;26,37,27;,
 3;37,39,27;,
 4;42,43,44,45;,
 4;43,46,47,44;,
 4;46,48,49,47;,
 4;48,50,51,49;,
 4;50,52,53,51;,
 4;52,54,55,53;,
 4;54,56,57,55;,
 4;56,58,59,57;,
 3;49,51,47;,
 3;51,53,47;,
 3;47,53,44;,
 3;53,55,44;,
 3;44,55,45;,
 3;55,57,45;;
 
 MeshMaterialList {
  1;
  34;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  23;
  -1.000000;0.000000;0.000004;,
  -0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000006;,
  1.000000;0.000000;0.000004;,
  0.707107;0.000000;0.707107;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;-0.707107;,
  -0.000000;0.000000;-1.000000;,
  -0.707107;0.000000;-0.707107;,
  -1.000000;0.000000;0.000000;,
  -0.707107;0.000000;0.707107;,
  0.000000;0.000000;1.000000;,
  0.707107;0.000000;0.707107;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;-0.707107;,
  -0.000000;0.000000;-1.000000;,
  -0.707107;0.000000;-0.707107;,
  -1.000000;0.000000;0.000000;,
  -0.707107;0.000000;0.707107;,
  -0.000000;0.000000;1.000000;,
  0.000000;-1.000000;0.000003;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;;
  34;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;20,20,20,20;,
  4;21,21,21,21;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,9,9,8;,
  4;9,10,10,9;,
  4;10,11,11,10;,
  4;11,4,4,11;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,15,15,14;,
  4;15,16,16,15;,
  4;16,17,17,16;,
  4;17,18,18,17;,
  4;18,19,19,18;,
  4;19,12,12,19;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;;
 }
 MeshTextureCoords {
  60;
  0.521419;0.931277;,
  0.928998;0.931277;,
  0.928998;0.523781;,
  0.521419;0.523781;,
  0.047029;0.481568;,
  0.967170;0.481568;,
  0.967170;0.022373;,
  0.047029;0.022373;,
  0.521391;0.930640;,
  0.929659;0.930640;,
  0.929659;0.523696;,
  0.521391;0.523696;,
  0.520973;0.932489;,
  0.929006;0.932489;,
  0.929006;0.525210;,
  0.520973;0.525210;,
  0.520557;0.931747;,
  0.930249;0.931747;,
  0.930249;0.523296;,
  0.520557;0.523296;,
  0.520961;0.931276;,
  0.929347;0.931276;,
  0.929347;0.523072;,
  0.520961;0.523072;,
  0.102685;0.907280;,
  0.132138;0.907280;,
  0.132138;0.552962;,
  0.102685;0.552962;,
  0.161590;0.907280;,
  0.161590;0.552962;,
  0.191043;0.907280;,
  0.191043;0.552962;,
  0.220496;0.907280;,
  0.220496;0.552962;,
  0.249948;0.907280;,
  0.249948;0.552962;,
  0.279401;0.907280;,
  0.281175;0.552962;,
  0.308854;0.905507;,
  0.308854;0.552962;,
  0.338307;0.907280;,
  0.338307;0.552962;,
  0.102685;0.907280;,
  0.132138;0.907280;,
  0.132138;0.552962;,
  0.102685;0.552962;,
  0.161590;0.907280;,
  0.161590;0.552962;,
  0.191043;0.907280;,
  0.191043;0.552962;,
  0.220496;0.907280;,
  0.220496;0.552962;,
  0.249948;0.907280;,
  0.249948;0.552962;,
  0.279401;0.907280;,
  0.279401;0.552962;,
  0.308854;0.907280;,
  0.308854;0.552962;,
  0.338307;0.907280;,
  0.338307;0.552962;;
 }
}
