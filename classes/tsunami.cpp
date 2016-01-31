double valor()
{

	if (tempo > period){
		tempo = 0;
	} else { 
		tempo += 0.1;
	}

	switch(tipo)
	{
		case DEGRAU:
			return this->amp + this->offset;
			break;

		case SENOIDAL:
			return amp * sin((tempo / period) * 2 * M_PI) + this->offset;
			break;

		case QUADRADA:
			if (tempo < period/2.f)
				return this->amp + this->offset;
			else
				return -this->amp + this->offset;
			break;

		case SERRA:
			return (2 * amp / period) * tempo - amp + this->offset;
			break;

		case ALEATORIO:
			if (tempo == 0) {
				srand (time(NULL));	
				period = ((float) (rand()) / (float) (RAND_MAX)) * (period_sup - period_inf) + period_inf;
				amp = ((float) (rand()) / (float) (RAND_MAX)) * (amp_sup - amp_inf) + amp_inf;
			}
			return this->amp;
			break;
	}
}