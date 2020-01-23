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
 0.45261;0.64215;7.84977;,
 2.65680;9.96567;6.24990;,
 -17.51040;29.34933;3.34611;,
 -18.98091;23.41569;4.39239;,
 2.00490;9.01059;-5.29344;,
 -17.93463;28.73583;-4.12215;,
 -0.19932;-0.31431;-3.69360;,
 -19.40424;22.80132;-3.07611;,
 0.45261;0.64215;7.84977;,
 -18.98091;23.41569;4.39239;,
 0.45261;0.64215;7.84977;,
 -0.19932;-0.31431;-3.69360;,
 2.65680;9.96567;6.24990;,
 -37.57375;49.23708;1.49544;;
 
 9;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;10,11,4,12;,
 3;3,2,13;,
 3;2,5,13;,
 3;5,7,13;,
 3;7,9,13;;
 
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
  0.019094;0.166861;0.985796;,
  0.042207;0.162258;0.985845;,
  0.701066;0.706345;-0.097896;,
  0.958978;0.272912;-0.076676;,
  -0.266419;-0.102320;-0.958411;,
  -0.181904;-0.125586;-0.975264;,
  0.065296;0.157565;0.985348;,
  -0.764349;-0.637633;0.095889;,
  -0.789249;-0.606726;0.094702;,
  0.971195;-0.235650;-0.035337;,
  0.705154;0.702284;-0.097747;,
  -0.348812;-0.078238;-0.933921;,
  -0.812906;-0.574862;0.093366;;
  9;
  4;0,0,1,1;,
  4;3,3,2,2;,
  4;5,5,4,4;,
  4;7,7,8,8;,
  4;9,9,3,3;,
  3;1,1,6;,
  3;2,2,10;,
  3;4,4,11;,
  3;8,8,12;;
 }
 MeshTextureCoords {
  14;
  0.250000;0.769940;,
  0.375000;0.769940;,
  0.437500;0.658590;,
  0.375000;0.658590;,
  0.500000;0.769940;,
  0.500000;0.658590;,
  0.625000;0.769940;,
  0.562500;0.658590;,
  0.750000;0.769940;,
  0.625000;0.658590;,
  0.500000;0.992660;,
  0.750000;0.881300;,
  0.250000;0.881300;,
  0.500000;0.547230;;
 }
}
