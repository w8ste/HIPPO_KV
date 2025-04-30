class HIPPOKV {
  public:
  HIPPOKV();

  ~HIPPOKV();

  // Insert key value pair overwriting existing value
  void hippo_put();

  // Insert key value pair not overwriting existing value
  void hippo_put_shallow();

  bool hippo_get();

};
