addpath('src');

if ~exist('data', 'dir')
    error('Data folder not found! Please check your directory structure.');
end

original = imread('data/board2.png');
crnobela = rgb2gray(original);

Templejti_path = {'data/Belakraljica.png', 'data/Belikonj.png', 'data/Belikralj.png', 'data/Belilovac.png', 'data/Belipesak.png', ...
    'data/Belitop.png', 'data/Crnakraljica.png', 'data/Crnikonj.png', 'data/Crnikralj.png', 'data/Crnilovac.png', 'data/Crnipesak.png', 'data/Crnitop.png'};
Templejti = cell(1, length(Templejti_path));

FEN_slova = {'Q','N','K','B','P','R','q','n','k','b','p','r'};

% Učitavanje templejta
for i = 1:length(Templejti_path)
    Templejti{i} = imread(Templejti_path{i});
end


% Priprema
broj_redova = size(crnobela, 1);
broj_kolona = size(crnobela, 2);

if(broj_redova~=720)
k=broj_kolona
r=broj_redova

n = 1;
m = 1;
while crnobela(n, k) == 0
    
    if(n==broj_kolona)
        n=1
        k=k-1
    end
    n = n + 1;
end

while crnobela(r, m) == 0

    if(m==broj_redova)
        m=1
        r=r-1
    end

    m = m + 1;
end

b = abs(r - n);
a = abs(k - m);

ugao_rotiranja = 90 - (atan(b / a) * 57.296);
uspravno = imrotate(crnobela, ugao_rotiranja);
broj_redova2 = size(uspravno, 1);
broj_kolona2 = size(uspravno, 2);
k = floor((broj_redova2 - 720) / 2);
iseceno = uspravno(k+1:k+720, k+1:k+720);

else
iseceno=crnobela

end

%imshow(crnobela)
%imshow(uspravno)
%imshow(iseceno)
%hold on

Pozicije =char(zeros(8, 8)); % Inicijalizacija matrice za pozicije figura


temp=iseceno
tmp_grey = Templejti{3};
flag=0

for m=0:7
    for n=0:7
        segment=temp(m*90+1:(m+1)*90,n*90+1:(n+1)*90)%uzima se da je slika 720x720, znaci jedno polje 90x90
        srednjavrednost=mean2(segment(30:60,30:60))
        if(srednjavrednost~=segment(10,10))
                
              
                c = normxcorr2(tmp_grey, segment);
            
                % Uklapanje dimenzija
                startRow = floor((size(c, 1) - 90) / 2) + 1;
                startCol = floor((size(c, 2) - 90) / 2) + 1;
                c_cropped = c(startRow:startRow+89, startCol:startCol+89);
            
                threshold = 0.8;
                [red, kolona] = find(c_cropped > threshold);
            
                if(isempty(red)==0)
                
                    orijentacija=temp
                    flag=1
                    
                end
        end
    end
end
temp=imrotate(temp,90)

imshow(temp)
hold on

if(flag~=1)
for m=0:7
    for n=0:7
        segment=temp(m*90+1:(m+1)*90,n*90+1:(n+1)*90)%uzima se da je slika 720x720, znaci jedno polje 90x90
        srednjavrednost=mean2(segment(30:60,30:60))
        if(srednjavrednost~=segment(10,10))
                
              
                c = normxcorr2(tmp_grey, segment);
            
                % Uklapanje dimenzija
                startRow = floor((size(c, 1) - 90) / 2) + 1;
                startCol = floor((size(c, 2) - 90) / 2) + 1;
                c_cropped = c(startRow:startRow+89, startCol:startCol+89);
            
                threshold = 0.8;
                [red, kolona] = find(c_cropped > threshold);
            
                if(isempty(red)==0)
              
                    orijentacija=temp
                    flag=1
                    
                end
        end
    end
end
temp=imrotate(temp,90)
end

imshow(temp)
hold on

if(flag~=1)
for m=0:7
    for n=0:7
        segment=temp(m*90+1:(m+1)*90,n*90+1:(n+1)*90)%uzima se da je slika 720x720, znaci jedno polje 90x90
        srednjavrednost=mean2(segment(30:60,30:60))
        if(srednjavrednost~=segment(10,10))
                
              
                c = normxcorr2(tmp_grey, segment);
            
                % Uklapanje dimenzija
                startRow = floor((size(c, 1) - 90) / 2) + 1;
                startCol = floor((size(c, 2) - 90) / 2) + 1;
                c_cropped = c(startRow:startRow+89, startCol:startCol+89);
            
                threshold = 0.8;
                [red, kolona] = find(c_cropped > threshold);
            
                if(isempty(red)==0)
                
                    orijentacija=temp
                    flag=1
                end
        end
    end
end
end

if(flag~=1)
    orijentacija=imrotate(temp,90)

end

imshow(orijentacija)
hold on

%dim_orijentacija=size(orijentacija)
%dim_x=dim_orijentacija(1)/8;
%dim_y=dim_orijentacija(2)/8;%ovo bi trebalo da se iskoristi dole umesto m i n



for m=0:7
    for n=0:7
        segment=orijentacija(m*90+1:(m+1)*90,n*90+1:(n+1)*90)%uzima se da je slika 720x720, znaci jedno polje 90x90
        srednja_vrednost=mean2(segment(30:60,30:60))
        if(srednja_vrednost~=segment(10,10))
            for i = 1:length(Templejti)
                tmp_grey = Templejti{i};
              
                c = normxcorr2(tmp_grey, segment);
            
                % Uklapanje dimenzija
                startRow = floor((size(c, 1) - 90) / 2) + 1;
                startCol = floor((size(c, 2) - 90) / 2) + 1;
                c_cropped = c(startRow:startRow+89, startCol:startCol+89);
            
                threshold = 0.75;
                [red, kolona] = find(c_cropped > threshold);
            
                if(isempty(red)==0)
               
                
                    Pozicije(m+1, n+1) = FEN_slova{i};
               

                end
            end
        end
    end
end
fengenerator(Pozicije)
