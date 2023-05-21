class Mode
{
  public:
    virtual Mode() {}
    virtual void Setup();
    virtual void Loop();

    virtual void Trigger5();
    virtual void Trigger6();
    virtual void Trigger7();
    virtual void Trigger8();
};
