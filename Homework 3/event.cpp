#include <iostream>
#include <string>
using namespace std;

class Event {
    public:
        Event(string s) { m_name = s; }
        string name() const { return m_name; }
        virtual bool isSport() const = 0; 
        virtual string need() const = 0;
        virtual ~Event() {}
    private:
        string m_name;
};

class BasketballGame: public Event {
    public:
        BasketballGame(string s) : Event(s) { }
        bool isSport() const { return true; }
        string need() const { return "hoops"; }
        ~BasketballGame() { cout << "Destroying the " << name() << " basketball game" << endl; }
};

class Concert: public Event {
    public:
        Concert(string s, string g) : Event(s) { m_genre = g; }
        bool isSport() const { return false; }
        string need() const { return "a stage"; }
        string genre() const { return m_genre; }
        ~Concert() { cout << "Destroying the " << name() << " " << genre() << " concert" << endl; }
    private:
        string m_genre;
};

class HockeyGame: public Event {
     public:
        HockeyGame(string s) : Event(s) { }
        bool isSport() const { return true; }
        string need() const { return "ice"; }
        ~HockeyGame() { cout << "Destroying the " << name() << " hockey game" << endl; }   
};


void display(const Event* e)
{
    cout << e->name() << ": ";
    if (e->isSport())
	cout << "(sport) ";
    cout << "needs " << e->need() << endl;
}

int main()
{
    Event* events[4];
    events[0] = new BasketballGame("Lakers vs. Suns");
      // Concerts have a name and a genre.
    events[1] = new Concert("Banda MS", "banda");
    events[2] = new Concert("KISS", "hard rock");
    events[3] = new HockeyGame("Kings vs. Flames");

    cout << "Here are the events." << endl;
    for (int k = 0; k < 4; k++)
	display(events[k]);

      // Clean up the events before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
	delete events[k];
}