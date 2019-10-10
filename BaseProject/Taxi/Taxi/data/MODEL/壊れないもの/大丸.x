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
 19;
 14.90025;0.00000;-47.85381;,
 -19.78303;0.00000;-47.85381;,
 -19.78303;38.53656;-47.85381;,
 14.90026;38.53656;-47.85378;,
 14.90026;38.53656;-47.85378;,
 -19.78303;38.53656;-47.85381;,
 14.75579;38.53656;37.51576;,
 -23.63629;38.53656;37.51576;,
 -23.63630;38.53656;-43.44510;,
 14.75579;38.53656;37.51576;,
 -23.63629;38.53656;37.51576;,
 -23.63629;0.00000;37.51576;,
 14.61131;0.00000;37.51582;,
 -23.63630;38.53656;-43.44510;,
 -23.63630;0.00000;-43.44510;,
 -23.63629;0.00000;37.51576;,
 -23.63629;38.53656;37.51576;,
 -23.63630;0.00000;-43.44510;,
 -23.63630;38.53656;-43.44510;;
 
 7;
 4;0,1,2,3;,
 3;4,5,6;,
 3;6,5,7;,
 3;5,8,7;,
 4;9,10,11,12;,
 4;13,14,15,16;,
 4;1,17,18,2;;
 
 MeshMaterialList {
  1;
  7;
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "‘åŠÛ.jpg";
   }
  }
 }
 MeshNormals {
  6;
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  -0.000001;0.000001;1.000000;,
  -0.936201;0.000000;-0.351465;,
  -0.413471;0.000000;-0.910517;,
  -1.000000;0.000000;0.000000;;
  7;
  4;0,4,4,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  4;2,2,2,2;,
  4;3,3,5,5;,
  4;4,3,3,4;;
 }
 MeshTextureCoords {
  19;
  0.679064;0.998267;,
  0.376023;0.998267;,
  0.376023;0.001409;,
  0.679064;0.001409;,
  0.066837;0.148422;,
  0.159533;0.148422;,
  0.066837;0.045426;,
  0.169832;0.045426;,
  0.169832;0.143103;,
  0.682806;0.001182;,
  0.995417;0.001182;,
  0.995417;0.999267;,
  0.682806;0.999267;,
  0.304104;0.001543;,
  0.304104;0.998443;,
  0.006698;0.998443;,
  0.006698;0.001543;,
  0.342356;0.998267;,
  0.342356;0.001409;;
 }
}
