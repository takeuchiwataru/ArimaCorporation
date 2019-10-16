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
 46;
 -1.29311;41.94709;-32.20506;,
 -0.78788;0.11674;1.19448;,
 -23.56554;42.29804;-21.80139;,
 -0.78788;0.11674;1.19448;,
 -31.95265;42.68789;1.30464;,
 -0.78788;0.11674;1.19448;,
 -21.54131;42.88831;23.57780;,
 -0.78788;0.11674;1.19448;,
 1.56959;42.78185;31.97080;,
 -0.78788;0.11674;1.19448;,
 23.84204;42.43090;21.56717;,
 -0.78788;0.11674;1.19448;,
 32.22913;42.04106;-1.53887;,
 -0.78788;0.11674;1.19448;,
 21.81780;41.84066;-23.81206;,
 -0.78788;0.11674;1.19448;,
 -1.29311;41.94709;-32.20506;,
 0.13825;42.36447;-0.11712;,
 -1.29311;41.94709;-32.20506;,
 -23.56554;42.29804;-21.80139;,
 -31.95265;42.68789;1.30464;,
 -21.54131;42.88831;23.57780;,
 1.56959;42.78185;31.97080;,
 23.84204;42.43090;21.56717;,
 32.22913;42.04106;-1.53887;,
 21.81780;41.84066;-23.81206;,
 -13.94928;132.38799;-13.85501;,
 13.84721;132.38799;-13.85501;,
 13.84721;38.05243;-13.85501;,
 -13.94928;38.05243;-13.85501;,
 13.84721;132.38799;-13.85501;,
 13.84721;132.38799;13.94149;,
 13.84721;38.05243;13.94149;,
 13.84721;38.05243;-13.85501;,
 13.84721;132.38799;13.94149;,
 -13.94928;132.38799;13.94149;,
 -13.94928;38.05243;13.94149;,
 13.84721;38.05243;13.94149;,
 -13.94928;132.38799;13.94149;,
 -13.94928;132.38799;-13.85501;,
 -13.94928;38.05243;-13.85501;,
 -13.94928;38.05243;13.94149;,
 13.84721;132.38799;-13.85501;,
 -13.94928;132.38799;-13.85501;,
 -13.94928;38.05243;-13.85501;,
 13.84721;38.05243;-13.85501;;
 
 22;
 3;0,1,2;,
 3;2,3,4;,
 3;4,5,6;,
 3;6,7,8;,
 3;8,9,10;,
 3;10,11,12;,
 3;12,13,14;,
 3;14,15,16;,
 3;17,18,19;,
 3;17,19,20;,
 3;17,20,21;,
 3;17,21,22;,
 3;17,22,23;,
 3;17,23,24;,
 3;17,24,25;,
 3;17,25,18;,
 4;26,27,28,29;,
 4;30,31,32,33;,
 4;34,35,36,37;,
 4;38,39,40,41;,
 4;38,31,42,43;,
 4;44,45,32,41;;
 
 MeshMaterialList {
  1;
  22;
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
   0.000000;0.800000;0.034510;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  25;
  -0.041664;-0.623723;-0.780534;,
  -0.593252;-0.607998;-0.527627;,
  -0.806035;-0.590184;0.044631;,
  -0.548418;-0.580884;0.601508;,
  0.029206;-0.585835;0.809904;,
  0.581599;-0.601967;0.547155;,
  0.784536;-0.619542;-0.025913;,
  0.526118;-0.628434;-0.572949;,
  -0.717533;-0.651552;-0.246224;,
  0.009482;0.999865;-0.013429;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.312017;-0.646463;0.696226;,
  0.009482;0.999865;-0.013429;,
  0.009482;0.999865;-0.013429;,
  0.009482;0.999865;-0.013429;,
  0.009482;0.999865;-0.013429;,
  0.009483;0.999865;-0.013429;,
  0.009482;0.999865;-0.013428;,
  0.009482;0.999865;-0.013428;,
  0.009481;0.999865;-0.013429;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;;
  22;
  3;0,8,1;,
  3;1,8,2;,
  3;2,8,3;,
  3;3,13,4;,
  3;4,13,5;,
  3;5,13,6;,
  3;6,7,7;,
  3;7,7,0;,
  3;9,14,15;,
  3;9,15,16;,
  3;9,16,17;,
  3;9,17,18;,
  3;9,18,19;,
  3;9,19,20;,
  3;9,20,21;,
  3;9,21,14;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;12,12,12,12;,
  4;22,22,22,22;,
  4;23,23,23,23;,
  4;24,24,24,24;;
 }
 MeshTextureCoords {
  46;
  0.000000;1.000000;,
  0.062500;0.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.187500;0.000000;,
  0.375000;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.312500;0.000000;,
  0.625000;1.000000;,
  0.375000;0.000000;,
  0.750000;1.000000;,
  0.437500;0.000000;,
  0.875000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}