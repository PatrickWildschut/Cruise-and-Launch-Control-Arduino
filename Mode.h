class Mode
{
  public:
    virtual Mode() {}
    virtual void Setup();
    virtual void Loop();
    virtual void Trigger7();
    virtual void Trigger8();
};
