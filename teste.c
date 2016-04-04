//tempos
	void tempoSubida(){
		if (setPoint - oldSetPoint > 0){
			if(!flagTrI && nivel_coringa >= oldSetPoint + dados.getFatInf() * deltaSetPoint)
			{
				t_subida_i = sinal.getTempo() - tSetPoint - 0.2;
				flagTrI = true;
			}
			if (!flagTrF && nivel_coringa >= oldSetPoint + dados.getFatSup() * deltaSetPoint){
				t_subida_f = sinal.getTempo() - tSetPoint - 0.2;
				flagTrF = true;
			}
			if (flagTrI && flagTrF)
			{
				t_subida = t_subida_f - t_subida_i;
				dados.gettSubida().setText(String.valueOf(t_subida));
				flagSubida = true;
			}
		}else{
			if(!flagTrI && nivel_coringa <= oldSetPoint + dados.getFatInf() * deltaSetPoint)
			{
				t_subida_i = sinal.getTempo() - tSetPoint - 0.2;
				flagTrI = true;
			}
			if (!flagTrF && nivel_coringa <= oldSetPoint + dados.getFatSup() * deltaSetPoint){
				t_subida_f = sinal.getTempo() - tSetPoint - 0.2;
				flagTrF = true;
			}
			if (flagTrI && flagTrF)
			{
				t_subida = t_subida_f - t_subida_i;
				dados.gettSubida().setText(String.valueOf(t_subida));
				flagSubida = true;
			}
		}
	}

	void tempoPico(){

		if(!flagPico){
			if (setPoint - oldSetPoint > 0){
				if (nivel_coringa < nivel_passado && nivel_coringa > setPoint)
				{
					t_pico = sinal.getTempo() - tSetPoint - 0.2;
					dados.gettPico().setText(String.valueOf(t_pico));
					nivel_pico = nivel_passado;

					if (dados.isPicoAbs())
						dados.getNivelPico().setText(String.valueOf((nivel_pico - setPoint)));
					else
						dados.getNivelPico().setText(String.valueOf(100 * (nivel_pico - setPoint)/(setPoint - oldSetPoint)));
					flagPico = true;
				}
			}
			else
			{
				//System.out.println("outro");
				if (nivel_coringa > nivel_passado && nivel_coringa < setPoint)
				{
					t_pico = sinal.getTempo() - 0.2 - tSetPoint;
					dados.gettPico().setText(String.valueOf(t_pico));
					nivel_pico = nivel_passado;
					if (dados.isPicoAbs())
						dados.gettPico().setText(String.valueOf(t_pico));
					else
						dados.getNivelPico().setText(String.valueOf(100 * (nivel_pico - setPoint)/(setPoint - oldSetPoint)));

					flagPico = true;
				}
			}
		}else{
			if (setPoint - oldSetPoint > 0){
				if (nivel_coringa > nivel_passado && nivel_coringa > setPoint && nivel_coringa > nivel_pico){
					flagPico = false;
				}
			}else{
				if (nivel_coringa < nivel_passado && nivel_coringa < setPoint && nivel_coringa < nivel_pico){
					flagPico = false;
				}
			}
		}
	}

	void settleTempo(){

		double nivelDeComparacao = 0;
		if (dados.isFaixa2())
			nivelDeComparacao = setPoint*0.02;

		if (dados.isFaixa5())
			nivelDeComparacao = setPoint*0.05;

		if (dados.isFaixa7())
			nivelDeComparacao = setPoint*0.07;

		if (dados.isFaixa10())
			nivelDeComparacao = setPoint*0.1;

		if(!flagSettleTempo){
			if(nivel_coringa <= nivelDeComparacao + setPoint && nivel_coringa >= setPoint - nivelDeComparacao ){
					settleTempo = sinal.getTempo() - 0.1;
					flagSettleTempo = true;
			}
		}else if(flagSettleTempo){

			if(!(nivel_coringa <= nivelDeComparacao + setPoint && nivel_coringa >= setPoint - nivelDeComparacao )){
				flagSettleTempo = false;

			}
		}

		if(flagSettleTempo)
			dados.gettAcomoda().setText(String.valueOf(settleTempo));
		else{dados.gettAcomoda().setText("");
		}
	}

	void sp_mudou(){
		//t_pico = t_subida = t_acomoda = 0;
		tSetPoint = sinal.getTempo() - 0.1;
		flagPico = flagSettleTempo = flagSubida = false;
		oldSetPoint = setPoint;
		setPoint = newSetPoint;
		deltaSetPoint = setPoint - oldSetPoint;

		flagTrI = false;
		flagTrF = false;

		dados.gettSubida().setText("");
		dados.gettAcomoda().setText("");
		dados.gettPico().setText("");
		dados.getNivelPico().setText("");
	}
