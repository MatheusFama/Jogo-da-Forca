#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <string>

#include <cppitertools/itertools.hpp>

#include <cstdlib>
#include <ctime>
void OpenGLWindow::initializeGL() {

  abcg::glClearColor(0, 0, 0, 1);
  restart();
}


Letra** OpenGLWindow:: InicializaPalavra()
{
  
  //std::string p1("Casa"); 
  Letra **arrayLetras;
  arrayLetras = (Letra **)malloc(palavra.palavra.length() * sizeof(Letra *));
  for(long i = 0; i < palavra.palavra.length(); i++)
  {
   Letra *letra = new Letra();
    letra->revelado = false;
    letra->mascara = "_";
    letra->letra = palavra.palavra.at(i);
    arrayLetras[i] =  letra;
  }

  return arrayLetras;
}

void OpenGLWindow::restart()
{
    unsigned seed = time(0);

    srand(seed);

    palavra = listaDePalavras[seed%12];

    letras = InicializaPalavra();
    chancesTotais = 5;

   chances = chancesTotais;
   encontrado = true;
   adivinhou = false;
   apertouBotaoAdivinhar = false;
   gameState = GameState::Play;

}

void OpenGLWindow::check()
{
  if(apertouBotaoAdivinhar && adivinhou)
  {
    gameState = GameState::Win;
    return;
  }

  if(apertouBotaoAdivinhar && !adivinhou)
  {
    gameState = GameState::Lose;
    return;
  }

  if(chances == 0)
  {
    gameState = GameState::Lose;
    return;
  }

  int cont =0;

  for(int i = 0; i < palavra.palavra.length(); i++)
  {
    if(letras[i]->revelado)
      cont++;
  } 

  if(cont == palavra.palavra.length())
  {
    gameState = GameState::Win;
    return;
  } 

}

void OpenGLWindow::paintGL() { abcg::glClear(GL_COLOR_BUFFER_BIT); }

void OpenGLWindow::cleanBuffer(char *buffer)
{
  for(int i = 0 ; i <IM_ARRAYSIZE(buffer); i++ )
  {
    buffer[i] = '\0';
  }
}
void OpenGLWindow::paintUI() {

  const auto appWindowWidth{static_cast<float>(getWindowSettings().width)};
  const auto appWindowHeight{static_cast<float>(getWindowSettings().height)};

  // "Forca" window
  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    const auto flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize};
    ImGui::Begin("Forca", nullptr, flags);
  
    // Menu
    {
      bool restartSelected{};
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Game")) {
          ImGui::MenuItem("Restart", nullptr, &restartSelected);
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }
      if (restartSelected) restart();
    }

 
    ImGui::Spacing();
    ImGui::Spacing();
    {

      std::string placar;
       switch (gameState)
       {

        case GameState::Play :
          placar = "Chances:"+ std::to_string(chances)+ "/" + std::to_string(chancesTotais);
          break;
        case GameState::Lose :
          placar = "Voce perdeu";
          break;
        case GameState :: Win:
          placar = "Voce ganhou";
          break;
       }
      
      ImGui::LabelText("###Chances",placar.c_str());

    }

    ImGui::Spacing();
    ImGui::Spacing();
    {
       ImGui::LabelText("###Dica",("Dica: " + palavra.dica).c_str());
    }

    ImGui::Spacing();
    ImGui::Spacing();
    // Tabuleiro
    const auto gridHeight{appWindowHeight - 22 - 58 - (palavra.palavra.length() * 10) - 58};

    for (auto j : iter::range(palavra.palavra.length())) {
        ImGui::Columns(palavra.palavra.length());
        ImGui::Button( letras[j]->revelado? letras[j]->letra.c_str() : letras[j]->mascara.c_str(), ImVec2(gridHeight / palavra.palavra.length(),gridHeight /  palavra.palavra.length()));
    
         ImGui::NextColumn();
      }

    ImGui::Columns(1);
    ImGui::Spacing();
    ImGui::Spacing();
    
     static char letter[2] = "";
    //Textbox
    {
     ImGui::PushItemWidth(200);
     ImGui::InputText("###txtLetra", letter, IM_ARRAYSIZE(letter));

    }
     ImGui::SameLine();
    {
       std::string txtButtonLetra = "Adivinhar Letra";
        ImGui::Button(txtButtonLetra.c_str(), ImVec2(300,gridHeight / txtButtonLetra.length()));
        if(ImGui::IsItemClicked() && gameState == GameState::Play)
        {
          bool encontrado = false;
          for (auto j : iter::range(palavra.palavra.length())) 
          {
              if(letras[j]->letra.compare(0,1,letter,0,1) == 0)
              {
                  letras[j]->revelado = true;
                  encontrado = true;
              }

          }

          if(!encontrado)
              chances--;

         cleanBuffer(letter);
        }
    }
    
    ImGui::Columns(1);
    ImGui::Spacing();
    ImGui::Spacing();

     static char word[10] = "";
    //Textbox
    {
     ImGui::PushItemWidth(200);
     ImGui::InputText("###txtPalavra", word, IM_ARRAYSIZE(word));
    }
    ImGui::SameLine();
    {
        std::string txtBuntonPalavra = "Adivinhar Palavra";
        ImGui::Button(txtBuntonPalavra.c_str(), ImVec2(300,gridHeight / txtBuntonPalavra.length()));
        if(ImGui::IsItemClicked() && gameState == GameState::Play)
        {
          apertouBotaoAdivinhar = true;
          if( palavra.palavra.compare(0,palavra.palavra.length(),word) == 0)
          {
            adivinhou = true;
          }
        

         cleanBuffer(word);
        }
    }

    if(adivinhou)
    {
       for (auto j : iter::range(palavra.palavra.length())) 
          {
            letras[j]->revelado = true;
          }

    }
    ImGui::Columns(1);
    ImGui::Spacing();
    ImGui::Spacing();
    check();
    // Botao de Restart
    {
      if (ImGui::Button("Restart game", ImVec2(-1, 50.0f))) {
        restart();
      }
    }

    ImGui::End();
  }
}
