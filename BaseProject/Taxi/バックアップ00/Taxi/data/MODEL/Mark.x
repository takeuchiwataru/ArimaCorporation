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
 26;
 0.00000;22.51651;-14.60901;,
 10.33012;22.51651;-10.33012;,
 0.00000;-39.26325;-0.00000;,
 14.60901;22.51651;0.00000;,
 0.00000;-39.26325;-0.00000;,
 10.33012;22.51651;10.33012;,
 0.00000;-39.26325;-0.00000;,
 -0.00000;22.51651;14.60901;,
 0.00000;-39.26325;-0.00000;,
 -10.33013;22.51651;10.33012;,
 0.00000;-39.26325;-0.00000;,
 -14.60901;22.51651;-0.00000;,
 0.00000;-39.26325;-0.00000;,
 -10.33012;22.51651;-10.33012;,
 0.00000;-39.26325;-0.00000;,
 0.00000;22.51651;-14.60901;,
 0.00000;-39.26325;-0.00000;,
 0.00000;40.83430;0.00000;,
 10.33012;22.51651;-10.33012;,
 0.00000;22.51651;-14.60901;,
 14.60901;22.51651;0.00000;,
 10.33012;22.51651;10.33012;,
 -0.00000;22.51651;14.60901;,
 -10.33013;22.51651;10.33012;,
 -14.60901;22.51651;-0.00000;,
 -10.33012;22.51651;-10.33012;;
 
 16;
 3;0,1,2;,
 3;1,3,4;,
 3;3,5,6;,
 3;5,7,8;,
 3;7,9,10;,
 3;9,11,12;,
 3;11,13,14;,
 3;13,15,16;,
 3;17,18,19;,
 3;17,20,18;,
 3;17,21,20;,
 3;17,22,21;,
 3;17,23,22;,
 3;17,24,23;,
 3;17,25,24;,
 3;17,19,25;;
 
 MeshMaterialList {
  1;
  16;
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
   0.650000;0.000000;0.020392;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.160000;0.000000;0.005020;;
  }
 }
 MeshNormals {
  15;
  -0.000000;0.224759;-0.974414;,
  0.689015;0.224759;-0.689015;,
  0.974414;0.224759;0.000000;,
  0.689015;0.224759;0.689015;,
  -0.000000;0.224759;0.974414;,
  -0.689015;0.224759;0.689015;,
  -0.974414;0.224759;-0.000000;,
  -0.689015;0.224759;-0.689015;,
  0.688129;-0.230123;-0.688129;,
  0.681402;0.675299;-0.282246;,
  0.688129;-0.230123;0.688129;,
  -0.688129;-0.230123;0.688129;,
  -0.688129;-0.230123;-0.688129;,
  -0.282246;0.675299;0.681402;,
  -0.552823;0.623517;-0.552823;;
  16;
  3;0,1,8;,
  3;1,2,8;,
  3;2,3,10;,
  3;3,4,10;,
  3;4,5,11;,
  3;5,6,11;,
  3;6,7,12;,
  3;7,0,12;,
  3;9,1,0;,
  3;9,2,1;,
  3;9,3,2;,
  3;13,4,3;,
  3;13,5,4;,
  3;13,6,5;,
  3;14,7,6;,
  3;14,0,7;;
 }
 MeshTextureCoords {
  26;
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.062500;0.000000;,
  0.250000;1.000000;,
  0.125000;0.000000;,
  0.375000;1.000000;,
  0.187500;0.000000;,
  0.500000;1.000000;,
  0.250000;0.000000;,
  0.625000;1.000000;,
  0.312500;0.000000;,
  0.750000;1.000000;,
  0.375000;0.000000;,
  0.875000;1.000000;,
  0.437500;0.000000;,
  1.000000;1.000000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
