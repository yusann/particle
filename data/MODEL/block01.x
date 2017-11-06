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
 98;
 -0.45000;1.00000;0.45000;,
 0.45000;1.00000;0.45000;,
 0.45000;1.00000;-0.45000;,
 -0.45000;1.00000;-0.45000;,
 -0.45000;0.95000;-0.50000;,
 0.45000;0.95000;-0.50000;,
 0.45000;0.05000;-0.50000;,
 -0.45000;0.05000;-0.50000;,
 -0.45000;0.00000;-0.45000;,
 0.45000;0.00000;-0.45000;,
 0.45000;0.00000;0.45000;,
 -0.45000;0.00000;0.45000;,
 -0.45000;0.05000;0.50000;,
 0.45000;0.05000;0.50000;,
 0.45000;0.95000;0.50000;,
 -0.45000;0.95000;0.50000;,
 0.50000;0.95000;0.45000;,
 0.50000;0.05000;0.45000;,
 0.50000;0.05000;-0.45000;,
 0.50000;0.95000;-0.45000;,
 -0.50000;0.05000;0.45000;,
 -0.50000;0.95000;0.45000;,
 -0.50000;0.95000;-0.45000;,
 -0.50000;0.05000;-0.45000;,
 -0.45000;1.00000;-0.45000;,
 0.45000;1.00000;-0.45000;,
 0.45000;0.99330;-0.47500;,
 -0.45000;0.99330;-0.47500;,
 0.45000;0.97500;-0.49330;,
 -0.45000;0.97500;-0.49330;,
 0.45000;0.95000;-0.50000;,
 -0.45000;0.00670;-0.47500;,
 0.45000;0.00670;-0.47500;,
 0.45000;0.00000;-0.45000;,
 -0.45000;0.02500;-0.49330;,
 0.45000;0.02500;-0.49330;,
 -0.45000;0.05000;-0.50000;,
 0.45000;0.05000;-0.50000;,
 -0.45000;0.99330;0.47500;,
 0.45000;0.99330;0.47500;,
 0.45000;1.00000;0.45000;,
 -0.45000;0.97500;0.49330;,
 0.45000;0.97500;0.49330;,
 -0.45000;0.95000;0.50000;,
 0.45000;0.95000;0.50000;,
 -0.45000;0.00000;0.45000;,
 0.45000;0.00000;0.45000;,
 0.45000;0.00670;0.47500;,
 -0.45000;0.00670;0.47500;,
 0.45000;0.02500;0.49330;,
 -0.45000;0.02500;0.49330;,
 0.45000;0.05000;0.50000;,
 0.47500;0.99330;0.45000;,
 0.47500;0.99330;-0.45000;,
 0.49330;0.97500;0.45000;,
 0.49330;0.97500;-0.45000;,
 0.50000;0.95000;-0.45000;,
 0.47500;0.00670;-0.45000;,
 0.47500;0.00670;0.45000;,
 0.49330;0.02500;-0.45000;,
 0.49330;0.02500;0.45000;,
 0.50000;0.05000;-0.45000;,
 0.50000;0.05000;0.45000;,
 -0.47500;0.99330;-0.45000;,
 -0.47500;0.99330;0.45000;,
 -0.49330;0.97500;-0.45000;,
 -0.49330;0.97500;0.45000;,
 -0.50000;0.95000;-0.45000;,
 -0.50000;0.95000;0.45000;,
 -0.47500;0.00670;0.45000;,
 -0.47500;0.00670;-0.45000;,
 -0.49330;0.02500;0.45000;,
 -0.49330;0.02500;-0.45000;,
 -0.50000;0.05000;-0.45000;,
 0.47500;0.95000;-0.49330;,
 0.47500;0.05000;-0.49330;,
 0.49330;0.95000;-0.47500;,
 0.49330;0.05000;-0.47500;,
 0.47500;0.05000;0.49330;,
 0.47500;0.95000;0.49330;,
 0.49330;0.05000;0.47500;,
 0.49330;0.95000;0.47500;,
 -0.47500;0.05000;-0.49330;,
 -0.47500;0.95000;-0.49330;,
 -0.49330;0.05000;-0.47500;,
 -0.49330;0.95000;-0.47500;,
 -0.47500;0.95000;0.49330;,
 -0.47500;0.05000;0.49330;,
 -0.49330;0.95000;0.47500;,
 -0.49330;0.05000;0.47500;,
 0.48062;0.97500;-0.48062;,
 -0.48062;0.97500;-0.48062;,
 0.48062;0.02500;-0.48062;,
 0.48062;0.97500;0.48062;,
 -0.48062;0.02500;-0.48062;,
 0.48062;0.02500;0.48062;,
 -0.48062;0.97500;0.48062;,
 -0.48062;0.02500;0.48062;;
 
 114;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;27,26,28,29;,
 4;29,28,30,4;,
 4;31,32,33,8;,
 4;34,35,32,31;,
 4;36,37,35,34;,
 4;38,39,40,0;,
 4;41,42,39,38;,
 4;43,44,42,41;,
 4;45,46,47,48;,
 4;48,47,49,50;,
 4;50,49,51,12;,
 4;25,40,52,53;,
 4;53,52,54,55;,
 4;55,54,16,56;,
 4;46,33,57,58;,
 4;58,57,59,60;,
 4;60,59,61,62;,
 4;0,24,63,64;,
 4;64,63,65,66;,
 4;66,65,67,68;,
 4;8,45,69,70;,
 4;70,69,71,72;,
 4;72,71,20,73;,
 4;37,30,74,75;,
 4;75,74,76,77;,
 4;77,76,56,61;,
 4;44,51,78,79;,
 4;79,78,80,81;,
 4;81,80,62,16;,
 4;4,36,82,83;,
 4;83,82,84,85;,
 4;85,84,73,67;,
 4;12,43,86,87;,
 4;87,86,88,89;,
 4;89,88,68,20;,
 3;25,53,26;,
 3;53,55,90;,
 3;90,26,53;,
 3;26,90,28;,
 3;55,56,76;,
 3;76,90,55;,
 3;90,76,74;,
 3;74,28,90;,
 3;28,74,30;,
 3;27,63,24;,
 3;91,65,63;,
 3;63,27,91;,
 3;29,91,27;,
 3;85,67,65;,
 3;65,91,85;,
 3;83,85,91;,
 3;91,29,83;,
 3;4,83,29;,
 3;32,57,33;,
 3;92,59,57;,
 3;57,32,92;,
 3;35,92,32;,
 3;77,61,59;,
 3;59,92,77;,
 3;75,77,92;,
 3;92,35,75;,
 3;37,75,35;,
 3;39,52,40;,
 3;93,54,52;,
 3;52,39,93;,
 3;42,93,39;,
 3;81,16,54;,
 3;54,93,81;,
 3;79,81,93;,
 3;93,42,79;,
 3;44,79,42;,
 3;8,70,31;,
 3;70,72,94;,
 3;94,31,70;,
 3;31,94,34;,
 3;72,73,84;,
 3;84,94,72;,
 3;94,84,82;,
 3;82,34,94;,
 3;34,82,36;,
 3;46,58,47;,
 3;58,60,95;,
 3;95,47,58;,
 3;47,95,49;,
 3;60,62,80;,
 3;80,95,60;,
 3;95,80,78;,
 3;78,49,95;,
 3;49,78,51;,
 3;0,64,38;,
 3;64,66,96;,
 3;96,38,64;,
 3;38,96,41;,
 3;66,68,88;,
 3;88,96,66;,
 3;96,88,86;,
 3;86,41,96;,
 3;41,86,43;,
 3;48,69,45;,
 3;97,71,69;,
 3;69,48,97;,
 3;50,97,48;,
 3;89,20,71;,
 3;71,97,89;,
 3;87,89,97;,
 3;97,50,87;,
 3;12,87,50;;
 
 MeshMaterialList {
  1;
  114;
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
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
  }
 }
 MeshNormals {
  80;
  -0.129498;0.983087;0.129498;,
  0.129498;0.983087;0.129498;,
  -0.129498;0.983087;-0.129498;,
  0.129498;0.983087;-0.129498;,
  -0.129499;0.129499;-0.983087;,
  0.129499;0.129499;-0.983087;,
  -0.129499;-0.129499;-0.983087;,
  0.129499;-0.129499;-0.983087;,
  -0.129499;-0.983087;-0.129499;,
  0.129499;-0.983087;-0.129499;,
  -0.129499;-0.983087;0.129499;,
  0.129499;-0.983087;0.129499;,
  -0.129499;-0.129499;0.983087;,
  0.129499;-0.129499;0.983087;,
  -0.129499;0.129499;0.983087;,
  0.129499;0.129499;0.983087;,
  0.983087;0.129499;0.129499;,
  0.983087;-0.129499;0.129499;,
  0.983087;0.129499;-0.129499;,
  0.983087;-0.129499;-0.129499;,
  -0.983087;-0.129499;0.129499;,
  -0.983087;0.129499;0.129499;,
  -0.983087;-0.129499;-0.129499;,
  -0.983087;0.129499;-0.129499;,
  -0.196463;0.854631;-0.480633;,
  0.196463;0.854631;-0.480633;,
  -0.185046;0.468829;-0.863689;,
  0.185046;0.468829;-0.863689;,
  -0.196463;-0.854631;-0.480633;,
  0.196463;-0.854631;-0.480633;,
  -0.185046;-0.468829;-0.863688;,
  0.185046;-0.468829;-0.863688;,
  -0.196463;0.854631;0.480633;,
  0.196463;0.854631;0.480633;,
  -0.185046;0.468829;0.863689;,
  0.185046;0.468829;0.863689;,
  -0.196463;-0.854631;0.480633;,
  0.196463;-0.854631;0.480633;,
  -0.185046;-0.468829;0.863688;,
  0.185046;-0.468829;0.863688;,
  0.480633;0.854631;0.196463;,
  0.480633;0.854631;-0.196463;,
  0.863689;0.468829;0.185046;,
  0.863689;0.468829;-0.185046;,
  0.480633;-0.854631;0.196463;,
  0.480633;-0.854631;-0.196463;,
  0.863688;-0.468829;0.185046;,
  0.863688;-0.468829;-0.185046;,
  -0.480633;0.854631;0.196463;,
  -0.480633;0.854631;-0.196463;,
  -0.863689;0.468829;0.185046;,
  -0.863689;0.468829;-0.185046;,
  -0.480633;-0.854631;0.196463;,
  -0.480633;-0.854631;-0.196463;,
  -0.863688;-0.468829;0.185046;,
  -0.863688;-0.468829;-0.185046;,
  0.471443;-0.167156;-0.865911;,
  0.471443;0.167156;-0.865911;,
  0.865910;-0.167156;-0.471443;,
  0.865910;0.167156;-0.471443;,
  0.471443;-0.167156;0.865911;,
  0.471443;0.167156;0.865911;,
  0.865910;-0.167156;0.471443;,
  0.865910;0.167156;0.471443;,
  -0.471443;-0.167156;-0.865911;,
  -0.471443;0.167156;-0.865911;,
  -0.865910;-0.167156;-0.471443;,
  -0.865910;0.167156;-0.471443;,
  -0.471443;-0.167156;0.865911;,
  -0.471443;0.167156;0.865911;,
  -0.865910;-0.167156;0.471443;,
  -0.865910;0.167156;0.471443;,
  0.589214;0.552860;-0.589214;,
  -0.589214;0.552860;-0.589214;,
  0.589214;-0.552860;-0.589214;,
  0.589214;0.552860;0.589214;,
  -0.589214;-0.552860;-0.589214;,
  0.589214;-0.552860;0.589214;,
  -0.589214;0.552860;0.589214;,
  -0.589214;-0.552860;0.589214;;
  114;
  4;0,1,3,2;,
  4;4,5,7,6;,
  4;8,9,11,10;,
  4;12,13,15,14;,
  4;16,17,19,18;,
  4;20,21,23,22;,
  4;2,3,25,24;,
  4;24,25,27,26;,
  4;26,27,5,4;,
  4;28,29,9,8;,
  4;30,31,29,28;,
  4;6,7,31,30;,
  4;32,33,1,0;,
  4;34,35,33,32;,
  4;14,15,35,34;,
  4;10,11,37,36;,
  4;36,37,39,38;,
  4;38,39,13,12;,
  4;3,1,40,41;,
  4;41,40,42,43;,
  4;43,42,16,18;,
  4;11,9,45,44;,
  4;44,45,47,46;,
  4;46,47,19,17;,
  4;0,2,49,48;,
  4;48,49,51,50;,
  4;50,51,23,21;,
  4;8,10,52,53;,
  4;53,52,54,55;,
  4;55,54,20,22;,
  4;7,5,57,56;,
  4;56,57,59,58;,
  4;58,59,18,19;,
  4;15,13,60,61;,
  4;61,60,62,63;,
  4;63,62,17,16;,
  4;4,6,64,65;,
  4;65,64,66,67;,
  4;67,66,22,23;,
  4;12,14,69,68;,
  4;68,69,71,70;,
  4;70,71,21,20;,
  3;3,41,25;,
  3;41,43,72;,
  3;72,25,41;,
  3;25,72,27;,
  3;43,18,59;,
  3;59,72,43;,
  3;72,59,57;,
  3;57,27,72;,
  3;27,57,5;,
  3;24,49,2;,
  3;73,51,49;,
  3;49,24,73;,
  3;26,73,24;,
  3;67,23,51;,
  3;51,73,67;,
  3;65,67,73;,
  3;73,26,65;,
  3;4,65,26;,
  3;29,45,9;,
  3;74,47,45;,
  3;45,29,74;,
  3;31,74,29;,
  3;58,19,47;,
  3;47,74,58;,
  3;56,58,74;,
  3;74,31,56;,
  3;7,56,31;,
  3;33,40,1;,
  3;75,42,40;,
  3;40,33,75;,
  3;35,75,33;,
  3;63,16,42;,
  3;42,75,63;,
  3;61,63,75;,
  3;75,35,61;,
  3;15,61,35;,
  3;8,53,28;,
  3;53,55,76;,
  3;76,28,53;,
  3;28,76,30;,
  3;55,22,66;,
  3;66,76,55;,
  3;76,66,64;,
  3;64,30,76;,
  3;30,64,6;,
  3;11,44,37;,
  3;44,46,77;,
  3;77,37,44;,
  3;37,77,39;,
  3;46,17,62;,
  3;62,77,46;,
  3;77,62,60;,
  3;60,39,77;,
  3;39,60,13;,
  3;0,48,32;,
  3;48,50,78;,
  3;78,32,48;,
  3;32,78,34;,
  3;50,21,71;,
  3;71,78,50;,
  3;78,71,69;,
  3;69,34,78;,
  3;34,69,14;,
  3;36,52,10;,
  3;79,54,52;,
  3;52,36,79;,
  3;38,79,36;,
  3;70,20,54;,
  3;54,79,70;,
  3;68,70,79;,
  3;79,38,68;,
  3;12,68,38;;
 }
 MeshTextureCoords {
  98;
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
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
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
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
