void listAll(string path, const Class* c)  // two-parameter overload
{
    cout << path + c->name() << endl;
    for(int i = 0;i < c->subclasses().size();i++) {
        listAll(path + c->name() + "=>", c->subclasses()[i]);
    }
}
