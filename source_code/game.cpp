#include <iostream>
#include <gtkmm.h>
using namespace std;

// initializing player turns
int nextMark = 1;
int numOfMarks = 0;

// struct to create the spaces
typedef struct spaces {
    int mark; // 0 = empty, 1 = X, 2 = O
    Gtk::Button* button;
} spaces;

#define numOfSpacesPerRow 3 // 3x3 grid
spaces grid[numOfSpacesPerRow][numOfSpacesPerRow];

void checkForWinner(int row, int col); // winner function
void endGame(int checkMark, int typeOfWin); // reset function
void setLabels(string mark, int typeOfWin);
void onRestartButtonClicked(); // restart function

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv);

    // load css file
    auto cssProvider = Gtk::CssProvider::create();
    cssProvider->load_from_path("SoftwareCSS/styles.css");
    auto defaultScreen = Gdk::Screen::get_default();
    auto styleContext = Gtk::StyleContext::create();
    styleContext->add_provider_for_screen(defaultScreen, cssProvider, GTK_STYLE_PROVIDER_PRIORITY_USER);

    Gtk::ApplicationWindow window; // window
    window.set_default_size(1000, 800);
    window.set_resizable(false);
    window.set_position(Gtk::WindowPosition::WIN_POS_CENTER);
    window.get_style_context()->add_class("appWindow");

    Gtk::HeaderBar* header = Gtk::manage(new Gtk::HeaderBar); // header
    header->set_title("TicTacToe!");
    header->set_show_close_button(true);
    window.set_titlebar(*header);

    auto icon = Gdk::Pixbuf::create_from_file("SoftwareCSS/icon.png"); // icon
    window.set_icon(icon);

    auto restartButton = Gtk::manage(new Gtk::Button("Click to reset the board")); // restart button
    restartButton->get_style_context()->add_class("restartButton");
    restartButton->signal_clicked().connect(sigc::ptr_fun(&onRestartButtonClicked));
    header->pack_start(*restartButton);

    Gtk::Grid gridLayout; // grid to hold the buttons
    gridLayout.set_halign(Gtk::ALIGN_CENTER);
    gridLayout.set_valign(Gtk::ALIGN_CENTER);
    gridLayout.set_row_spacing(10);
    gridLayout.set_column_spacing(10);
    window.add(gridLayout);

    for (int i = 0; i < numOfSpacesPerRow; i++) {
        for (int j = 0; j < numOfSpacesPerRow; j++) {
            grid[i][j].mark = 0; // make it an empty space
            grid[i][j].button = Gtk::manage(new Gtk::Button());
            grid[i][j].button->set_label("");
            grid[i][j].button->get_style_context()->add_class("gameButton");
            gridLayout.attach(*grid[i][j].button, i, j, 1, 1);

            // button clicked signal connect
            grid[i][j].button->signal_clicked().connect([i, j] {
                string currentLabel = grid[i][j].button->get_label();
                if (nextMark == 1) {
                    grid[i][j].button->set_label("X");
                    grid[i][j].mark = 1;
                    nextMark = 2;
                } else if (nextMark == 2) {
                    grid[i][j].button->set_label("O");
                    grid[i][j].mark = 2;
                    nextMark = 1;
                }
                grid[i][j].button->get_style_context()->add_class("animBtn");
                grid[i][j].button->set_sensitive(false); // prevents the user from clicking a used space
                checkForWinner(i, j);
            });
        }
    }

    window.show_all();

    return app->run(window);
}

void checkForWinner(int row, int col) {
  int checkMark = grid[row][col].mark;
  numOfMarks++;

  // check rows
  for (int i = 0; i < numOfSpacesPerRow; i++) {
    if (grid[i][0].mark == checkMark && grid[i][1].mark == checkMark && grid[i][2].mark == checkMark) {
      endGame(checkMark, i + 1);
      return;
    }
  }

  // check columns
  for (int i = 0; i < numOfSpacesPerRow; i++) {
    if (grid[0][i].mark == checkMark && grid[1][i].mark == checkMark &&  grid[2][i].mark == checkMark) {
      endGame(checkMark, i + 4);
      return; 
    }
  }

  // check diagonals
  if (grid[0][0].mark == checkMark && grid[1][1].mark == checkMark && grid[2][2].mark == checkMark) {
    endGame(checkMark, 7);
    return;
  }
  if (grid[2][0].mark == checkMark && grid[1][1].mark == checkMark && grid[0][2].mark == checkMark) {
    endGame(checkMark, 8);
  }
}

void endGame(int checkMark, int typeOfWin) {
    // reset all buttons
    for(int i = 0; i < numOfSpacesPerRow; i++) {
      for(int j = 0; j < numOfSpacesPerRow; j++) {
        grid[i][j].button->set_sensitive(false);
        grid[i][j].button->set_label("");  
      }
    }
    // set labels for the winning line
    if (checkMark == 1) {
      setLabels("X", typeOfWin); 
    } else {
      setLabels("O", typeOfWin);
    }
}

void setLabels(string mark, int typeOfWin) {
  switch(typeOfWin) {
    case 1: // top column
      grid[0][0].button->set_label(mark);
      grid[0][1].button->set_label(mark);  
      grid[0][2].button->set_label(mark);
      break;
    case 2: // middle column
      grid[1][0].button->set_label(mark);
      grid[1][1].button->set_label(mark);  
      grid[1][2].button->set_label(mark);
      break;
    case 3: // bottom column
      grid[2][0].button->set_label(mark);
      grid[2][1].button->set_label(mark);
      grid[2][2].button->set_label(mark);
      break;
    case 4: // left row
      grid[0][0].button->set_label(mark);
      grid[1][0].button->set_label(mark);
      grid[2][0].button->set_label(mark);
      break;
    case 5: // middle row
      grid[0][1].button->set_label(mark);
      grid[1][1].button->set_label(mark);
      grid[2][1].button->set_label(mark);
      break;
    case 6: // right row
      grid[0][2].button->set_label(mark);
      grid[1][2].button->set_label(mark);
      grid[2][2].button->set_label(mark);
      break;
    case 7: // top-to-bottom diagonal
      grid[0][0].button->set_label(mark);
      grid[1][1].button->set_label(mark);
      grid[2][2].button->set_label(mark);
      break;
    case 8: // bottom-to-top diagonal
      grid[2][0].button->set_label(mark);
      grid[1][1].button->set_label(mark);  
      grid[0][2].button->set_label(mark);
      break;
  }
}

void onRestartButtonClicked() {
  for (int i = 0; i < numOfSpacesPerRow; i++) {
    for (int j = 0; j < numOfSpacesPerRow; j++) {
      grid[i][j].mark = 0;
      grid[i][j].button->set_label("");
      grid[i][j].button->set_sensitive(true);
    }
  }
}
