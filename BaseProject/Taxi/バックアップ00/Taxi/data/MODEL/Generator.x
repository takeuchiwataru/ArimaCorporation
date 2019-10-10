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
 28;
 51.58212;7.30199;20.37723;,
 51.58212;56.36677;20.37723;,
 44.49788;0.07502;20.37723;,
 -51.58212;56.36677;20.37723;,
 -51.58212;0.07502;20.37723;,
 -51.58212;0.07502;-20.37723;,
 -51.58212;56.36677;-20.37723;,
 44.49786;0.07502;-20.37722;,
 51.58212;56.36677;-20.37723;,
 51.58212;7.30199;-20.37723;,
 -51.58212;0.07502;20.37723;,
 -51.58212;56.36677;20.37723;,
 -51.58212;56.36677;-20.37723;,
 -51.58212;0.07502;-20.37723;,
 51.58212;7.30199;-20.37723;,
 51.58212;56.36677;-20.37723;,
 51.58212;56.36677;20.37723;,
 51.58212;7.30199;20.37723;,
 -51.58212;56.36677;20.37723;,
 51.58212;56.36677;20.37723;,
 51.58212;56.36677;-20.37723;,
 -51.58212;56.36677;-20.37723;,
 44.49788;0.07502;20.37723;,
 44.49786;0.07502;-20.37722;,
 44.49788;0.07502;20.37723;,
 -51.58212;0.07502;20.37723;,
 -51.58212;0.07502;-20.37723;,
 44.49786;0.07502;-20.37722;;
 
 11;
 3;0,1,2;,
 3;1,3,2;,
 3;3,4,2;,
 3;5,6,7;,
 3;6,8,7;,
 3;8,9,7;,
 4;10,11,12,13;,
 4;14,15,16,17;,
 4;18,19,20,21;,
 4;17,22,23,14;,
 4;24,25,26,27;;
 
 MeshMaterialList {
  1;
  11;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\gerador3.jpg";
   }
  }
 }
 MeshNormals {
  16;
  0.000000;0.000000;-1.000000;,
  0.000000;-0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  -0.000000;-0.000000;1.000000;,
  -0.000001;0.000000;1.000000;,
  -0.000000;-0.000000;1.000000;,
  -0.000001;0.000000;-1.000000;,
  -0.000000;-0.000000;-1.000000;,
  -0.000000;-0.000000;-1.000000;,
  -0.000000;-0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.925776;-0.378072;-0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.387285;-0.921960;-0.000000;,
  0.000000;-1.000000;0.000000;;
  11;
  3;4,5,9;,
  3;5,3,9;,
  3;3,2,9;,
  3;0,1,8;,
  3;1,7,8;,
  3;7,6,8;,
  4;10,10,10,10;,
  4;11,12,12,11;,
  4;13,13,13,13;,
  4;11,14,14,11;,
  4;14,15,15,14;;
 }
 MeshTextureCoords {
  28;
  0.374863;0.688622;,
  0.082016;0.688622;,
  0.417999;0.646339;,
  0.082016;0.072876;,
  0.417999;0.072876;,
  0.483290;0.423403;,
  0.483290;0.166445;,
  0.921872;0.423403;,
  0.954210;0.166445;,
  0.954210;0.390414;,
  0.795273;0.979861;,
  0.795273;0.574104;,
  0.501541;0.574104;,
  0.501541;0.979861;,
  0.625732;0.390414;,
  0.625732;0.166445;,
  0.811767;0.166445;,
  0.811767;0.390414;,
  0.811767;0.059464;,
  0.811767;0.530384;,
  0.625732;0.530384;,
  0.625733;0.059464;,
  0.811767;0.423403;,
  0.625732;0.423403;,
  0.811767;0.091802;,
  0.811767;0.530384;,
  0.625733;0.530384;,
  0.625732;0.091802;;
 }
}
