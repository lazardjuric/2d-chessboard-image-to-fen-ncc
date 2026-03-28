function fen = fengenerator(matrica)
    
    fen = ''; % inicijalizacija FEN

    for i = 1:size(matrica, 1) % 1 - red
        praznaPolja = 0; % brojac praznih polja
        red = '';    % inicijalizacija reda

        for j = 1:size(matrica, 2) % 2 - kolona
            element = matrica(i, j);
            if element == 0
                praznaPolja = praznaPolja + 1;
            else
                if praznaPolja > 0
                    red = [red, num2str(praznaPolja)]; % dodavanje praznih polja u red
                    praznaPolja = 0; 
                end
                red = [red, element]; % dodavanje oznaka u red
            end
        end

        % praznja polja na kraju reda
        if praznaPolja > 0
            red = [red, num2str(praznaPolja)];
        end

        fen = [fen, red, '/'];
    end

    fen = fen(1:end-1);
end
