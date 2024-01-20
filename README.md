# Drone2024

ドローン制御用のプログラムです  
visual studio code   
platform io   
esp32 - arduino  
srcの中のファイルmain.cpp以外消してから実行  

  モーター配置  
BNO055

    1 --f-- 2  
    |   B   |
    |   N   |  
    |   O   |  
    3 --b-- 4  

プロポ割り当て  
Futaba 6EX  

     #   5               #   6

         -               +
         2               3
    左右    4-      回転    1+
         前              上  
         後              下