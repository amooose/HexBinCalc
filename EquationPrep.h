String hbresult[3];

//No Stroll on arduino, quickest way I could think of int64->str
String i64Str(int64_t n)
{
    int64_t tn = n;
    int i = 0;
    bool neg = false;
	char buf[20];
    if(tn==0){
        buf[i++] = '0';
    }
    if(tn<0){
        neg=true;
        tn*=-1;
    }
    while (tn)
    {
        buf[i++]= '0' + (tn % 10);
        tn /= 10;
    }
    if(neg){
		buf[i++]='-';
    }
	buf[i]='\0';
    return tn == 0 ? String(rev(buf,i)) : "\0";
}

bool findHexBinStr(String x, char id)
{
	int s = -1, e = 0;
	bool found = false;
	for (int i = 0; i < x.length(); i++) {
		if (x[i] == id && id == 'b' && i != x.length() - 1) {
			if (!isoperator(x[i + 1]) && x[i + 1] != ')') {
				s = -1; e = 0;
			}
		}
		if (x[i] == id && e != 0) {
			hbresult[0] = String(s,DEC);
			hbresult[1] = String(i,DEC);
			hbresult[2] = x.substring(s, s+e);
			return true;
		}
		if (isxdigit(x[i])) {
			e++;
			if (s == -1) {
				s = i;
			}
		}
		else if (s != -1) {
			s = -1; e = 0;
		}

	}
	return false;
}

//Hex String to decimal
String hexStrToDec(String hexString) {
  
  int64_t decValue = 0;
  char nextInt;
  for ( short i = 0; i < hexString.length(); i++ ) {
    nextInt = toupper(hexString[i]);
    if( isxdigit(nextInt) ) {
        if (nextInt >= '0' && nextInt <= '9') nextInt = nextInt - '0';
        if (nextInt >= 'A' && nextInt <= 'F') nextInt = nextInt - 'A' + 10;
        decValue = (decValue << 4) + nextInt;
    }
  }
  return i64Str(decValue);
}

//Binary String to decimal
String binStrToDec(String s) {
	char* p;
	int64_t n = atoll(s.c_str(), 2);
	return i64Str(n);
}

//Convert all 0x___ Hex values to ___h format for processing
String prepHexStr(String equation ) {
	equation.replace(" ","");
	while (equation.indexOf("0x") >= 0) {
		int hX = equation.indexOf("0x");
		int hEnd = -1;
		int index = 0;
		if (hX >= 0) { hX++; }
		for (index = hX; index < equation.length(); index++) {
			if ((isoperator(equation[index]) || equation[index] == ')')) {
				hEnd = index;
				break;
			}
		}

		//our hex is at the end of a String
		if (hEnd == -1 && index == equation.length()) {
			hEnd = index;
		}
		if (hEnd > 0) {
			String h1 = equation.substring(0, hX - 1);
			String h2 = equation.substring(hEnd, equation.length());
			equation = h1 + equation.substring(hX + 1, hEnd) + 'h' + h2;
		}
	}
	return equation;
}

//Convert all Hex or Binary String within the equation to Decimal.
String equationHexBinToDec(String equation, char dataType) {

	while (findHexBinStr(equation, dataType)) {
		String h1 = equation.substring(0, hbresult[0].toInt());
		String h2 = equation.substring(hbresult[1].toInt() + 1, equation.length());
		if(dataType == 'b'){
			equation = h1 + binStrToDec(hbresult[2]) + h2;
		} else{
			equation = h1 + hexStrToDec(hbresult[2]) + h2;
			
		}
	}
	return equation;
}

String tryConv(String equation)
{
	
	//Serial.println("EQUATION: " + equation);
	//Prep equation first, remove hex ambiguity
	equation=prepHexStr(equation);
	Serial.println("PREPPED: " + equation);
	
	equation=equationHexBinToDec(equation,'b');
	
	equation=equationHexBinToDec(equation,'h');

	//Serial.println("DEC CONVERSION EQ: " + equation);
	return equation;
}
