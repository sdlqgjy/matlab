% X ԭʼ���ݾ���
X=xlsread('imputed.xlsx','Sheet1','D2:BA29');

sumZi = sqrt(sum(X .^ 2));
sizeX = size(X);
Zij = X - X;

% Ȩ��
w=xlsread('50������ָ��;ٰ��ļ����˻�����ϵ��.xlsx','Sheet1','B2:AY2'); 

% �����Ȩ�淶���󣬽�������������һ������ȡZij
for i = 1 : sizeX(2)
    Zij(:, i) = X(:, i) / sumZi(i) * w(i);
end

Zplus = max(Zij);
Zminus = min(Zij);

Dplus_i = zeros(sizeX(1), 1);
Dminus_i = Dplus_i;

Zplus_ij = X - X;
Zminus_ij = X - X;

for i = 1 : sizeX(2)
    for j = 1 : sizeX(1)
        Zplus_ij(j, i) = (Zplus(i) - Zij(j, i)) ^ 2;
        Zminus_ij(j, i) = (Zminus(i) - Zij(j, i)) ^ 2;
    end
end

for i = 1 : sizeX(1)
    Dplus_i(i) = sqrt(sum(Zplus_ij(i, :)));
    Dminus_i(i) = sqrt(sum(Zminus_ij(i, :)));
end

Ci = Dminus_i ./ (Dminus_i + Dplus_i);
% ������״ͼ
figure;
bar(Ci);

% ��ӱ�ǩ�ͱ���
xlabel('sample number');
ylabel('Ci value');
title('The comprehensive index of each sample obtained by the Topsis method');






