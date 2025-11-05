#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAX_PLAYLISTS = 50;
const int MAX_ITEMS = 50;

class Media {
public:
    virtual void display() const = 0;
    virtual string toFileString() const = 0;
    virtual Media* clone() const = 0;
    virtual ~Media() {}
};

class Song : public Media {
    string title, artist;
    float duration;
public:
    Song() : duration(0.0f) {}
    Song(string t, string a, float d) { title = t; artist = a; duration = d; }

    void input() {
        cout << "Enter song title: ";
        getline(cin, title);
        cout << "Enter artist name: ";
        getline(cin, artist);
        cout << "Enter duration (minutes): ";
        cin >> duration;
        cin.ignore();
    }

    void display() const override {
        cout << " • " << title << " [" << artist << "] - " << duration << " min\n";
    }

    string toFileString() const override {
        return "SONG," + title + "," + artist + "," + to_string(duration) + "\n";
    }

    Media* clone() const override { return new Song(*this); }
};

class MediaCollection {
protected:
    string name;
    Media* items[MAX_ITEMS];
    int itemCount;
public:
    MediaCollection(string n = "Collection") : name(n), itemCount(0) {}

    virtual void addItem(Media* m) {
        if (itemCount < MAX_ITEMS) items[itemCount++] = m;
        else cout << "Collection full!\n";
    }

    virtual void displayItems() const {
        cout << "\n--- " << name << " ---\n";
        if (itemCount == 0) cout << "(No media items)\n";
        else for (int i = 0; i < itemCount; i++) items[i]->display();
    }

    void saveToFile() const {
        ofstream f(name + ".txt");
        for (int i = 0; i < itemCount; i++) f << items[i]->toFileString();
        cout << "Saved to " << name << ".txt\n";
    }

    virtual ~MediaCollection() {
        for (int i = 0; i < itemCount; i++) delete items[i];
    }
};

class Playlist : public MediaCollection {
public:
    Playlist(string n = "Playlist") : MediaCollection(n) {}

    void addSongInteractive() {
        Song* s = new Song;
        s->input();
        addItem(s);
    }

    void removeLast() {
        if (itemCount > 0) {
            delete items[--itemCount];
            cout << "Removed last song.\n";
        } else {
            throw "Playlist is empty!";
        }
    }

    Playlist operator+(const Playlist& other) {
        Playlist merged("Merged Playlist");
        for (int i = 0; i < itemCount; i++) merged.addItem(items[i]->clone());
        for (int j = 0; j < other.itemCount; j++) merged.addItem(other.items[j]->clone());
        return merged;
    }

    friend void comparePlaylists(const Playlist&, const Playlist&);
};

void comparePlaylists(const Playlist& a, const Playlist& b) {
    cout << "\nComparison: " << a.name << " vs " << b.name << "\n";
    if (a.itemCount > b.itemCount) cout << a.name << " has more songs.\n";
    else if (a.itemCount < b.itemCount) cout << b.name << " has more songs.\n";
    else cout << "Both have the same number of songs.\n";
}

template <typename T>
void showDetails(const T& info) {
    cout << "[DETAIL] " << info << endl;
}

void printMenu() {
    cout << "\n====== PLAYLIST MENU ======\n";
    cout << "1) Create new playlist\n";
    cout << "2) Add song to playlist\n";
    cout << "3) Display playlist\n";
    cout << "4) Compare two playlists\n";
    cout << "5) Merge and display two playlists\n";
    cout << "6) Remove last song from playlist\n";
    cout << "7) Save playlist to file\n";
    cout << "8) Exit\n";
    cout << "Enter choice: ";
}

int main() {
    Playlist* playlists[MAX_PLAYLISTS];
    int playlistCount = 0;
    int choice;

    try {
        do {
            printMenu();
            cin >> choice;
            cin.ignore();

            if (choice == 1 && playlistCount < MAX_PLAYLISTS) {
                string name;
                cout << "Enter playlist name: ";
                getline(cin, name);
                playlists[playlistCount++] = new Playlist(name);
                cout << "Playlist \"" << name << "\" created.\n";

            } else if (choice == 2) {
                int index;
                cout << "Enter playlist index (0-" << playlistCount - 1 << "): ";
                cin >> index;
                cin.ignore();
                if (index >= 0 && index < playlistCount) 
                    playlists[index]->addSongInteractive();
                else cout << "Invalid index!\n";

            } else if (choice == 3) {
                int index;
                cout << "Enter playlist index (0-" << playlistCount - 1 << "): ";
                cin >> index;
                cin.ignore();
                if (index >= 0 && index < playlistCount) 
                    playlists[index]->displayItems();
                else cout << "Invalid index!\n";

            } else if (choice == 4) {
                int a, b;
                cout << "Enter index of playlist A: ";
                cin >> a;
                cout << "Enter index of playlist B: ";
                cin >> b;
                cin.ignore();
                if (a >= 0 && b >= 0 && a < playlistCount && b < playlistCount) 
                    comparePlaylists(*playlists[a], *playlists[b]);
                else cout << "Invalid indexes!\n";

            } else if (choice == 5) {
                int a, b;
                cout << "Merge playlist index A: ";
                cin >> a;
                cout << "with playlist index B: ";
                cin >> b;
                cin.ignore();
                if (a >= 0 && b >= 0 && a < playlistCount && b < playlistCount) {
                    Playlist merged = *playlists[a] + *playlists[b];
                    merged.displayItems();
                } else cout << "Invalid indexes!\n";

            } else if (choice == 6) {
                int index;
                cout << "Enter playlist to remove song from (0-" << playlistCount - 1 << "): ";
                cin >> index;
                cin.ignore();
                if (index >= 0 && index < playlistCount) 
                    playlists[index]->removeLast();
                else cout << "Invalid playlist index!\n";

            } else if (choice == 7) {
                int index;
                cout << "Enter playlist index to save: ";
                cin >> index;
                cin.ignore();
                if (index >= 0 && index < playlistCount) 
                    playlists[index]->saveToFile();
                else cout << "Invalid index!\n";

            } else if (choice != 8) {
                cout << "Invalid option!\n";
            }

        } while (choice != 8);
    } 
    catch (const char* errMsg) {
        cout << "Error: " << errMsg << endl;
    }

    for (int i = 0; i < playlistCount; i++) delete playlists[i]; // cleanup

    showDetails("Program complete. Thx for using Playlist Manager!");
    return 0;
}
