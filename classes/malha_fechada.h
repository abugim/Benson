#ifndef _MF_H
#define _MF_H

class Malha_fechada : public controle {
public:
  Malha_fechada ();
  ~Malha_fechada ();
private:
  double *referencia;
};

#endif
