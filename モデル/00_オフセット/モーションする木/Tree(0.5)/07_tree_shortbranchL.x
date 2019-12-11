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
 14;
 -0.74232;1.59415;10.95267;,
 31.64698;39.55000;5.19037;,
 29.19613;49.43940;3.44657;,
 -4.41593;17.13330;8.28622;,
 29.90318;48.41690;-9.00053;,
 -3.32942;15.54150;-10.95267;,
 32.35253;38.52605;-7.25713;,
 0.34428;0.00000;-8.28628;,
 31.64698;39.55000;5.19037;,
 -0.74232;1.59415;10.95267;,
 -0.74232;1.59415;10.95267;,
 -4.41593;17.13330;8.28622;,
 0.34428;0.00000;-8.28628;,
 62.63497;82.58570;0.36212;;
 
 9;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;10,11,5,12;,
 3;1,13,2;,
 3;2,13,4;,
 3;4,13,6;,
 3;6,13,8;;
 
 MeshMaterialList {
  1;
  9;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "tree000.jpg";
   }
  }
 }
 MeshNormals {
  13;
  -0.019094;0.166861;0.985795;,
  -0.042207;0.162258;0.985845;,
  -0.958976;0.272920;-0.076677;,
  -0.701061;0.706350;-0.097896;,
  0.181904;-0.125586;-0.975264;,
  0.266420;-0.102320;-0.958411;,
  -0.065296;0.157565;0.985348;,
  0.764338;-0.637646;0.095891;,
  0.789245;-0.606732;0.094704;,
  -0.971196;-0.235649;-0.035338;,
  -0.705155;0.702283;-0.097747;,
  0.348812;-0.078237;-0.933921;,
  0.812907;-0.574861;0.093366;;
  9;
  4;0,1,1,0;,
  4;2,3,3,2;,
  4;4,5,5,4;,
  4;7,8,8,7;,
  4;9,2,2,9;,
  3;1,6,1;,
  3;3,10,3;,
  3;5,11,5;,
  3;8,12,8;;
 }
 MeshTextureCoords {
  14;
  0.250000;0.769940;,
  0.375000;0.658590;,
  0.437500;0.658590;,
  0.375000;0.769940;,
  0.500000;0.658590;,
  0.500000;0.769940;,
  0.562500;0.658590;,
  0.625000;0.769940;,
  0.625000;0.658590;,
  0.750000;0.769940;,
  0.500000;0.992660;,
  0.250000;0.881300;,
  0.750000;0.881300;,
  0.500000;0.547230;;
 }
}
