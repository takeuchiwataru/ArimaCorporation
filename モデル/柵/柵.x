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
 24;
 1.02540;-0.21369;16.45251;,
 -1.51828;-0.23585;16.41610;,
 -0.78978;-0.18158;-16.62150;,
 1.75390;-0.15943;-16.58508;,
 1.57987;16.46074;-16.48541;,
 -0.96381;16.43858;-16.52182;,
 -1.69231;16.38432;16.51578;,
 0.85137;16.40648;16.55219;,
 0.88029;16.40266;16.61986;,
 -1.66338;16.38049;16.58346;,
 -1.51828;-0.23585;16.41610;,
 1.02540;-0.21369;16.45251;,
 1.75390;-0.15943;-16.58508;,
 -0.78978;-0.18158;-16.62150;,
 -0.93489;16.43476;-16.45415;,
 1.60878;16.45692;-16.41773;,
 1.02540;-0.21369;16.45251;,
 1.75390;-0.15943;-16.58508;,
 1.60878;16.45692;-16.41773;,
 0.88029;16.40266;16.61986;,
 -0.77322;-0.18144;-16.62126;,
 -1.50171;-0.23571;16.41634;,
 -1.64683;16.38065;16.58369;,
 -0.91833;16.43490;-16.45391;;
 
 6;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;;
 
 MeshMaterialList {
  1;
  6;
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
    "C:\\Users\\game107\\Desktop\\É`Å[ÉÄ ÉÇÉfÉã ë≤ã∆\\çÚ\\çÚ.png";
   }
  }
 }
 MeshNormals {
  6;
  0.008730;-0.999961;-0.001450;,
  -0.008733;0.999961;0.001450;,
  -0.014225;-0.010194;0.999847;,
  0.014225;0.010194;-0.999847;,
  -0.999720;-0.008508;-0.022058;,
  0.999720;0.008509;0.022058;;
  6;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;5,5,5,5;,
  4;4,4,4,4;;
 }
 MeshTextureCoords {
  24;
  0.002087;0.899317;,
  0.001429;0.997719;,
  1.000246;0.997726;,
  1.000696;0.899325;,
  0.001572;0.903193;,
  0.005036;0.999649;,
  1.002983;0.999175;,
  1.001102;0.902720;,
  -0.000372;0.827516;,
  0.073792;0.827516;,
  0.073792;-0.000637;,
  -0.000372;-0.000637;,
  0.002642;0.822722;,
  0.073853;0.822722;,
  0.073853;-0.004194;,
  0.002642;-0.004194;,
  0.004026;0.822970;,
  1.002512;0.822970;,
  1.002512;0.002220;,
  0.004026;0.002220;,
  0.002616;0.824370;,
  0.999480;0.824370;,
  0.999480;0.000519;,
  0.002616;0.000519;;
 }
}
