#Configurações do Executavel
project(JogoForca)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp)

enable_abcg(${PROJECT_NAME})
