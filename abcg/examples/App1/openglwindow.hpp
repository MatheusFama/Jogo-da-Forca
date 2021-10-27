//definição da classe
#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>
#include <string>
#include <array>
#include "abcg.hpp"
class Letra {
  public: //tudo abaixo é público
    Letra(){}; //note só a assinatura do construtor (poderia ser inline também)
    bool revelado;
    std::string mascara;
    std::string letra;

}; 

class Palavra
{
  public:
  std::string palavra;
  std::string dica;
};


class OpenGLWindow : public abcg::OpenGLWindow {
  private:
      enum class GameState { Play, Lose, Win}; 
      GameState gameState{GameState::Play};

 protected:
    //funções da nossa classe que deriva da OpenGlWindow
      void initializeGL() override;
      void paintGL() override;
      void paintUI() override;
      void check();
      void cleanBuffer(char *);
      Letra ** InicializaPalavra();
      bool encontrado;
      bool adivinhou;
      bool apertouBotaoAdivinhar;
      int chances;
      int chancesTotais;
      Letra ** letras;
      Palavra palavra;
      Palavra listaDePalavras[12] = 
      {
        {"empatia", "afeto"}
        ,{"conjuge", "companheiro"}
        ,{"prolixo", "verboso"}
        ,{"verbete", "nota"}
        ,{"familia", "parentes"}
        ,{"recesso", "intervalo"}
        ,{"sofisma", "mentira"}
        ,{"permear", "cruzar"}
        ,{"receoso", "ansioso"}
        ,{"impelir", "obrigar"}
        ,{"parecer", "assemelhar"}
        ,{"mostrar", "transparecer"}
      };

void restart();

};

//
#endif
