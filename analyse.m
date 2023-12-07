close all; clc; clear variables;

%% SMALL

for i = 1:65
originalImage = imread(strcat("in/imag_", int2str(i), ".png"));

out1b = imread(strcat("out/small/out_", int2str(i), "_1b.png"));
out2b = imread(strcat("out/small/out_", int2str(i), "_2b.png"));
out3b = imread(strcat("out/small/out_", int2str(i), "_3b.png"));
out4b = imread(strcat("out/small/out_", int2str(i), "_4b.png"));
out5b = imread(strcat("out/small/out_", int2str(i), "_5b.png"));
out6b = imread(strcat("out/small/out_", int2str(i), "_6b.png"));
out7b = imread(strcat("out/small/out_", int2str(i), "_7b.png"));
out8b = imread(strcat("out/small/out_", int2str(i), "_8b.png"));

er1(i) = immse(out1b, originalImage);
er2(i) = immse(out2b, originalImage);
er3(i) = immse(out3b, originalImage);
er4(i) = immse(out4b, originalImage);
er5(i) = immse(out5b, originalImage);
er6(i) = immse(out6b, originalImage);
er7(i) = immse(out7b, originalImage);
er8(i) = immse(out8b, originalImage);

ps1(i) = 10*log10((255^2)/er1(i));
ps2(i) = 10*log10((255^2)/er2(i));
ps3(i) = 10*log10((255^2)/er3(i));
ps4(i) = 10*log10((255^2)/er4(i));
ps5(i) = 10*log10((255^2)/er5(i));
ps6(i) = 10*log10((255^2)/er6(i));
ps7(i) = 10*log10((255^2)/er7(i));
ps8(i) = 10*log10((255^2)/er8(i));

img_p(i, 1) = ps1(i);
img_p(i, 2) = ps2(i);
img_p(i, 3) = ps3(i);
img_p(i, 4) = ps4(i);
img_p(i, 5) = ps5(i);
img_p(i, 6) = ps6(i);
img_p(i, 7) = ps7(i);
img_p(i, 8) = ps8(i);

img_e(i, 1) = er1(i);
img_e(i, 2) = er2(i);
img_e(i, 3) = er3(i);
img_e(i, 4) = er4(i);
img_e(i, 5) = er5(i);
img_e(i, 6) = er6(i);
img_e(i, 7) = er7(i);
img_e(i, 8) = er8(i);

end

%%

k = 1:65;
j = 1:8;

plot(j, img_p(k, j));
figure
plot(j, img_e(k, j));

fprintf("SMALL:\n");
fprintf("Diferença entre 1b e 2b: %d\n", mean(ps2(k)-ps1(k)))
fprintf("Diferença entre 2b e 3b: %d\n", mean(ps3(k)-ps2(k)))
fprintf("Diferença entre 3b e 4b: %d\n", mean(ps4(k)-ps3(k)))
fprintf("Diferença entre 4b e 5b: %d\n", mean(ps5(k)-ps4(k)))
fprintf("Diferença entre 5b e 6b: %d\n", mean(ps6(k)-ps5(k)))
fprintf("Diferença entre 6b e 7b: %d\n", mean(ps7(k)-ps6(k)))
fprintf("Diferença entre 7b e 8b: %d\n", mean(ps8(k)-ps7(k)))


%% BIG

for i = 1:65
originalImage = imread(strcat("in/imag_", int2str(i), ".png"));

out1b = imread(strcat("out/big/out_", int2str(i), "_1b.png"));
out2b = imread(strcat("out/big/out_", int2str(i), "_2b.png"));
out3b = imread(strcat("out/big/out_", int2str(i), "_3b.png"));
out4b = imread(strcat("out/big/out_", int2str(i), "_4b.png"));
out5b = imread(strcat("out/big/out_", int2str(i), "_5b.png"));
out6b = imread(strcat("out/big/out_", int2str(i), "_6b.png"));
out7b = imread(strcat("out/big/out_", int2str(i), "_7b.png"));
out8b = imread(strcat("out/big/out_", int2str(i), "_8b.png"));

er1(i) = immse(out1b, originalImage);
er2(i) = immse(out2b, originalImage);
er3(i) = immse(out3b, originalImage);
er4(i) = immse(out4b, originalImage);
er5(i) = immse(out5b, originalImage);
er6(i) = immse(out6b, originalImage);
er7(i) = immse(out7b, originalImage);
er8(i) = immse(out8b, originalImage);

ps1(i) = 10*log10((255^2)/er1(i));
ps2(i) = 10*log10((255^2)/er2(i));
ps3(i) = 10*log10((255^2)/er3(i));
ps4(i) = 10*log10((255^2)/er4(i));
ps5(i) = 10*log10((255^2)/er5(i));
ps6(i) = 10*log10((255^2)/er6(i));
ps7(i) = 10*log10((255^2)/er7(i));
ps8(i) = 10*log10((255^2)/er8(i));

img_p(i, 1) = ps1(i);
img_p(i, 2) = ps2(i);
img_p(i, 3) = ps3(i);
img_p(i, 4) = ps4(i);
img_p(i, 5) = ps5(i);
img_p(i, 6) = ps6(i);
img_p(i, 7) = ps7(i);
img_p(i, 8) = ps8(i);

img_e(i, 1) = er1(i);
img_e(i, 2) = er2(i);
img_e(i, 3) = er3(i);
img_e(i, 4) = er4(i);
img_e(i, 5) = er5(i);
img_e(i, 6) = er6(i);
img_e(i, 7) = er7(i);
img_e(i, 8) = er8(i);

end

%%

k = 1:65;
j = 1:8;

plot(j, img_p(k, j));
figure
plot(j, img_e(k, j));


fprintf("BIG:\n");
fprintf("Diferença entre 1b e 2b: %d\n", mean(ps2(k)-ps1(k)))
fprintf("Diferença entre 2b e 3b: %d\n", mean(ps3(k)-ps2(k)))
fprintf("Diferença entre 3b e 4b: %d\n", mean(ps4(k)-ps3(k)))
fprintf("Diferença entre 4b e 5b: %d\n", mean(ps5(k)-ps4(k)))
fprintf("Diferença entre 5b e 6b: %d\n", mean(ps6(k)-ps5(k)))
fprintf("Diferença entre 6b e 7b: %d\n", mean(ps7(k)-ps6(k)))
fprintf("Diferença entre 7b e 8b: %d\n", mean(ps8(k)-ps7(k)))

