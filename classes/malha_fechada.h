#ifndef _MF_H
#define _MF_H

#define REF 5
#define ERRO 6
#define ERRO_SAT 7

class Malha_fechada : public controle {
public:
  Malha_fechada ();
  ~Malha_fechada ();
private:
  double *referencia;
};

#endif
