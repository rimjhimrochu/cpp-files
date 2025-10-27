#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ====================== CLASS DEFINITION =========================

class Song {
private:
    string title;
    string artist;
    float duration; // in minutes

public:
    // --- Constructors ---
    Song() {
        title = "Unknown";
        artist = "Unknown";
        duration = 0.0;
    }

    Song(string t, string a, float d) {
        title = t;
        artist = a;
        duration = d;
    }

    // --- Destructor ---
    ~Song() {
        // optional message for debugging
        // cout << "Deleting song: " << title << endl;
    }

    // --- Member Functions ---
    void input() {
        cout << "Enter song title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter artist name: ";
        getline(cin, artist);
        cout << "Enter duration (in minutes): ";
        cin >> duration;
    }

    void display() const {
        cout << "Title: " << title << "\nArtist: " << artist
             << "\nDuration: " << duration << " min\n";
    }

    string getTitle() const { return title; }

    // --- Friend function for saving song to file ---
    friend void saveToFile(Song s);
};

// =================================================================

class Playlist {
private:
    Song *songs;
    int count;
    int capacity;

public:
    // --- Constructor ---
    Playlist(int cap = 10) {
        capacity = cap;
        count = 0;
        songs = new Song[capacity];
    }

    // --- Destructor ---
    ~Playlist() {
        delete[] songs;
    }

    // --- Add Song (Overloaded Function) ---
    void addSong(Song s) {
        if (count < capacity) {
            songs[count++] = s;
            cout << "Song added successfully!\n";
        } else {
            cout << "Playlist is full!\n";
        }
    }

    // Function Overloading Example
    void addSong(string title, string artist, float duration) {
        if (count < capacity) {
            songs[count++] = Song(title, artist, duration);
            cout << "Song added successfully using overloaded function!\n";
        } else {
            cout << "Playlist is full!\n";
        }
    }

    // --- Display Songs ---
    void showPlaylist() const {
        if (count == 0) {
            cout << "Playlist is empty.\n";
            return;
        }
        cout << "\n----- Your Playlist -----\n";
        for (int i = 0; i < count; i++) {
            cout << "\nSong " << i + 1 << ":\n";
            songs[i].display();
        }
    }

    // --- Search song by title ---
    void searchSong(string name) const {
        for (int i = 0; i < count; i++) {
            if (songs[i].getTitle() == name) {
                cout << "\nSong found!\n";
                songs[i].display();
                return;
            }
        }
        cout << "Song not found.\n";
    }

    // --- Save playlist to file ---
    void savePlaylist() {
        ofstream fout("playlist.txt", ios::app);
        if (!fout) {
            cout << "Error opening file!\n";
            return;
        }
        for (int i = 0; i < count; i++) {
            fout << "Title: " << songs[i].getTitle() << endl;
        }
        fout.close();
        cout << "Playlist saved to file successfully.\n";
    }
};

// =================================================================

// Friend function definition
void saveToFile(Song s) {
    ofstream fout("songs.txt", ios::app);
    if (fout) {
        fout << s.getTitle() << endl;
    }
    fout.close();
}

// =================================================================

// Base class for demonstrating Inheritance
class MusicSystem {
public:
    virtual void info() {
        cout << "\n--- Welcome to Music Playlist Manager ---\n";
    }
};

// Derived class (Inheritance + Virtual Function)
class Player : public MusicSystem {
public:
    void info() override { // Function Overriding Example
        cout << "\n🎶 Music Player Ready! 🎶\n";
    }
};

// =================================================================

// Template Example
template <class T>
void showInfo(T msg) {
    cout << msg << endl;
}

// =================================================================

// Exception Handling Example
void divideDemo() {
    int a, b;
    cout << "\nEnter two numbers (for exception demo): ";
    cin >> a >> b;
    try {
        if (b == 0)
            throw b;
        cout << "Result = " << (a / b) << endl;
    } catch (int) {
        cout << "Division by zero not allowed!\n";
    }
}

// =================================================================
// ======================== MAIN FUNCTION ==========================

int main() {
    MusicSystem *sys = new Player();
    sys->info(); // Dynamic Binding (virtual function)
    delete sys;

    Playlist playlist(5);
    int choice;

    do {
        cout << "\n=========== MUSIC PLAYLIST MANAGER ===========\n";
        cout << "1. Add Song\n2. Add Song (Overloaded)\n3. Show Playlist\n4. Search Song\n5. Save Playlist to File\n6. Divide Demo (Exception Handling)\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            Song s;
            s.input();
            playlist.addSong(s);
            saveToFile(s);
            break;
        }
        case 2: {
            string title, artist;
            float duration;
            cin.ignore();
            cout << "Enter title: ";
            getline(cin, title);
            cout << "Enter artist: ";
            getline(cin, artist);
            cout << "Enter duration: ";
            cin >> duration;
            playlist.addSong(title, artist, duration);
            break;
        }
        case 3:
            playlist.showPlaylist();
            break;

        case 4: {
            cin.ignore();
            string name;
            cout << "Enter song title to search: ";
            getline(cin, name);
            playlist.searchSong(name);
            break;
        }

        case 5:
            playlist.savePlaylist();
            break;

        case 6:
            divideDemo();
            break;

        case 0:
            cout << "Exiting Music Playlist Manager...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    // Template Example
    showInfo("\nThank you for using Music Playlist Manager!");

    return 0;
}

.