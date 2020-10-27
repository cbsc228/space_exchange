#include <iostream>
#include <sstream>
#include <time.h>
#include <cstdlib>
#include <string>
#include <map>
using namespace std;

//number of currencies supported by the script
const int NUM_CURRENCIES = 6;
//number of commodities supported by the script
const int NUM_COMMODITIES = 10;
//number of validation fields used
const int MAX_NUM_FIELDS = 3;

//returns a random value between minValue and maxValue
float simulate(float maxValue,float minValue){
	maxValue -= minValue;
	float randomValue = (maxValue * (((float) rand())/RAND_MAX)) + minValue;
	return randomValue;
}

//populates the currency name dictionary
void fillCurrencies(string currencyNames[]){
	currencyNames[0] = "usdollar";
	currencyNames[1] = "euro";
	currencyNames[2] = "xarn";
	currencyNames[3] = "icekrona";
	currencyNames[4] = "polandzloty";
	currencyNames[5] = "galacticrock";
}

//populates the exchange rates into the matrix
void fillExchangeRate(map< string, map<string, float> > &exchangeRates){
	map<string, float> usdollarRates;
	map<string, float> euroRates;
	map<string, float> xarnRates;
	map<string, float> icekronaRates;
	map<string, float> polandzlotyRates;
	
	usdollarRates["euro"] = 0.88;
	usdollarRates["xarn"] = 26.2;
	usdollarRates["icekrona"] = 119.88;
	usdollarRates["polandzloty"] = 3.76;
	usdollarRates["galacticrock"] = 0.123456;
	
	euroRates["xarn"] = 29.7727373;
	euroRates["icekrona"] = 136.227273;
	euroRates["polandzloty"] = 4.27;
	euroRates["galacticrock"] = 0.140291;
	
	xarnRates["icekrona"] = 4.57557;
	xarnRates["polandzloty"] = 0.1434198;
	xarnRates["galacticrock"] = 0.0047121;
	
	icekronaRates["polandzloty"] = 0.0313447;
	icekronaRates["galacticrock"] = 0.001029839;
	
	polandzlotyRates["galacticrock"] = 0.03285528;
	
	exchangeRates["usdollar"] = usdollarRates;
	exchangeRates["euro"] = euroRates;
	exchangeRates["xarn"] = xarnRates;
	exchangeRates["icekrone"] = icekronaRates;
	exchangeRates["polandzloty"] = polandzlotyRates;
	
}

//populates the commodity name dictionary (part of parallel array set)
void fillCommodities(string commodityNames[]){
	commodityNames[0] = "terrangold";
	commodityNames[1] = "terransilver";
	commodityNames[2] = "terrancopper";
	commodityNames[3] = "xarngold";
	commodityNames[4] = "xarnsilver";
	commodityNames[5] = "xarncopper";
	commodityNames[6] = "corn";
	commodityNames[7] = "wheat";
	commodityNames[8] = "coffee";
	commodityNames[9] = "xarnsugar";
}

//populates the commodity minimum value dictionary (part of parallel array set)
void fillCommodityMinValues(float commodityMinValues[]){
	commodityMinValues[0] = 1100.0;
	commodityMinValues[1] = 13.0;
	commodityMinValues[2] = 2.0;
	commodityMinValues[3] = 1900.0;
	commodityMinValues[4] = 113.0;
	commodityMinValues[5] = 0.1;
	commodityMinValues[6] = 338.25;
	commodityMinValues[7] = 438.50;
	commodityMinValues[8] = 101.01;
	commodityMinValues[9] = 191.01;
}

//populates the commodity maximum values dictionary (part of parallel array set)
void fillCommodityMaxValues(float commodityMaxValues[]){
	commodityMaxValues[0] = 1800.0;
	commodityMaxValues[1] = 18.0;
	commodityMaxValues[2] = 4.0;
	commodityMaxValues[3] = 2800.0;
	commodityMaxValues[4] = 158.0;
	commodityMaxValues[5] = 0.2;
	commodityMaxValues[6] = 440.00;
	commodityMaxValues[7] = 600.40;
	commodityMaxValues[8] = 1000000.0;
	commodityMaxValues[9] = 2000.0;
}

//parses the input provided by the GET method into its constituant parts
void parseInput(string &inputOneName, string &inputTwoName, string &inputThreeName,
				string &inputOneValue, string &inputTwoValue, string &inputThreeValue){
					
	string userInput = getenv("QUERY_STRING");
	string varName = "";
	string varValue = "";
	int varIndicator = 0;
	int i = 0;
	
	if(userInput.length() != 0){
		while(i < userInput.length()){
			varIndicator += 1;
			string varName = "";
			string varValue = "";
		
			while(userInput[i] != '='){
				varName = varName + userInput[i];
				i+= 1;
			}
			i+= 1;
			while((userInput[i] != '&') && (i < userInput.length())){
				varValue = varValue + userInput[i];
				i+= 1;
			}
			i+= 1;
		
			switch(varIndicator){
				case 1:
				inputOneName = varName;
				inputOneValue = varValue;
				break;
				case 2:
				inputTwoName = varName;
				inputTwoValue = varValue;
				break;
				case 3:
				inputThreeName = varName;
				inputThreeValue = varValue;
				break;
			}
		}
		
	}
}


//formats the strings to all be lower case
string formatInput(string inputString){
	string lowerString = "";
	for(int i = 0; i < inputString.length(); i++)
		lowerString = lowerString + char(tolower(inputString[i]));
	return lowerString;
}

//determines which type of table needs to be generated based on the provdided input
int determineAction(string &inputOneName, string &inputTwoName, string &inputThreeName){
	
	if((inputOneName == "inputCurrency") && (inputTwoName == "outputCurrency") && (inputThreeName == "amount"))
		return 0;
	else if (inputOneName == "commodityName")
		return 1;
	else
		return -1;
}

//OVERLOADED FUNCTION
//checks to see if the given input is in the database
//if the given input is valid returns 0
//if the given input is empty returns 1
//if the given input is unknown returns 2
int checkInput(string &input, string database[], int length){
	//empty input case
	if(input == ""){
		input = "missing";
		return 1;
	}
	//looks to see if input is in database
	for(int i = 0; i < length; i++)
		if(database[i] == input)
			return 0;
	//unknown input case
	input = "unknown choice";	
	return 2;
}

//OVERLOADED FUNCTION
//checks to see if the given amount (floating point number) is a valid number
//if the given amount is valid returns 0
//if the given amount is invalid returns 3
int checkInput(string &amount){
	stringstream convert(amount);
	int inputAmount = 0;
	
	if(convert >> inputAmount){
		if (inputAmount >= 0)
			return 0;
		else{
			amount = "invalid amount";
			return 3;
		}
	}
	else{
		amount = "invalid amount";
		return 3;
	}
}

//converts one provided currency into another provided currency
float convert(map <string, map<string, float> > exchangeRates, string inputCurrency, string outputCurrency, string amount){
	float result;
	float conversionFactor;
	
	stringstream convert(amount);
	int inputAmount = 0;
	convert >> inputAmount;
	
	//used to store whether the function needs to multiply or divide with the conversion factor
	//true indicates multiply
	//false indicates divide
	bool operatorIndicator = false;
	
	//looks to see if a to b exists where a is the input currency and b is the output currency
	//if found determines that the necessary operation is multiplication
	for(auto i = exchangeRates.begin(); i != exchangeRates.end(); i++)
		if (i->first == inputCurrency)
			if( (i->second).find(outputCurrency) != (i->second).end() ){
				operatorIndicator = true;
				conversionFactor = (i->second).find(outputCurrency)->second;
			}
		
	//looks to see if b to a exists where a is the input currency and b is the output currency
	//if found determines the necessary operation is division
	if(operatorIndicator == false){
		for(auto i = exchangeRates.begin(); i != exchangeRates.end(); i++)
			if (i->first == outputCurrency)
				if( (i->second).find(inputCurrency) != (i->second).end() ){
				operatorIndicator = false;
				conversionFactor = (i->second).find(inputCurrency)->second;
			}
	}
	
	//calculates the currency conversion
	if(operatorIndicator)
		result = inputAmount * conversionFactor;
	else
		result = inputAmount / conversionFactor;
	
	return result;
}

//looks to see if the provided commodity is one of those supported.
//If found returns the value of that commodity based on a minimum and maximum bound
float lookup(string commodityNames[], float commodityMinValues[], float commodityMaxValues[], string givenCommodity){
	int index = -1;
	float result;
	
	//finds the index of the given commodity if it exists in the database
	for(int i = 0; i < NUM_COMMODITIES; i++){
		if(commodityNames[i] == givenCommodity)
			index = i;
	}
	
	if (index != -1){
		//returns randomized value of given commodity
		result = simulate(commodityMaxValues[index], commodityMinValues[index]);
		return result;
	}
	else
		//returns 0.0 as a error message if the commodity doesn't exist
		return 0.0;
}

//determines the necessary color for table fields based on input validation
string determineColor(int colorCheck){
	switch(colorCheck){
		//indicates the value is missing
		case 1:
		return "red";
		
		//indicates the value is unknown
		case 2:
		return "blue";
		
		//indicates the value is invalid
		case 3:
		return "green";
		
		//used when the value is known and valid
		default:
		return "black";
	}
}

//prints the HTML document for currency conversion
void printConvertTable(string fieldOne, string fieldTwo, string fieldThree, float fieldFour, int fieldChecks[]){
	string fieldOneColor = determineColor(fieldChecks[0]);
	string fieldTwoColor = determineColor(fieldChecks[1]);
	string fieldThreeColor = determineColor(fieldChecks[2]);
	string fieldFourColor = determineColor(fieldChecks[3]);
	
	//prints "Invalid conversion" in the case that the input and output currency provided are the same
	if((fieldOne == fieldTwo) && (fieldOne != "missing") && (fieldTwo != "missing")){
		fieldOne = "";
		fieldTwo = "";
		fieldThree = "Invalid conversion";
	}
	//prints "Nothing submitted, nothing returned" in the case that no input is provided
	if(fieldFour == -1.0)
		fieldThree = "Nothing submitted, nothing returned";
	
	cout << "<!DOCTYPE html>" << endl;
	cout << "<html>" << endl;
	cout << "<head>" << endl;
	cout << 	"<style>" << endl;
	cout << 		"table, th {" << endl;
	cout << 			"border: 2px solid black;" << endl;
	cout << 		"}" << endl;
	cout << 		"#fieldOne{" << endl;
	cout <<			    "border: 2px solid " << fieldOneColor << endl;
	cout << 		"}" << endl;
	cout << 		"#fieldTwo{" << endl;
	cout << 			"border: 2px solid " << fieldTwoColor << endl;
	cout << 		"}" << endl;
	cout << 		"#fieldThree{" << endl;
	cout << 			"border: 2px solid " << fieldThreeColor << endl;
	cout << 		"}" << endl;
	cout << 		"#fieldFour{" << endl;
	cout << 			"border: 2px solid " << fieldFourColor << endl;
	cout << 		"}" << endl;
	cout <<			".blankSpace{" << endl;
	cout <<				"color: white " << endl;
	cout << 		"}" << endl;
	cout << 	"</style>" << endl;
	cout << "</head>" << endl;
	cout << "<table>" << endl;
	cout << 	"<tr>" << endl;
	cout << 		"<th>" << endl;
	cout << 			"<h2>" << endl;
	cout << 				"In Currency" << endl;
	cout << 			"</h2>" << endl;
	cout << 		"</th>" << endl;
	cout << 		"<th>" << endl;
	cout << 			"<h2>" << endl;
	cout << 				"Out Currency" << endl;
	cout << 			"</h2>" << endl;
	cout << 		"</th>"	<< endl;
	cout << 		"<th>" << endl;
	cout << 			"<h2>" << endl;
	cout << 				"Quantity" << endl;
	cout << 			"</h2>" << endl;
	cout << 		"</th>" << endl;
	cout << 		"<th>" << endl;
	cout << 			"<h2 class='blankSpace'>" << endl;
	cout << 				"BLANK" << endl;
	cout << 			"</h2>" << endl;
	cout << 		"</th>" << endl;
	cout << 		"<th>" << endl;
	cout << 			"<h2>" << endl;
	cout << 				"Result" << endl;
	cout << 			"</h2>" << endl;
	cout << 		"</th>" << endl;
	cout << 	"</tr>" << endl;
	cout << 		"<tr>" << endl;
	cout << 		"<th id = 'fieldOne'>" << endl;
	cout << 			"<h2>" << endl;
	cout << 				fieldOne << endl;
	cout << 			"</h2>" << endl;
	cout << 		"</th>" << endl;
	cout << 		"<th id = 'fieldTwo'>" << endl;
	cout << 			"<h2>" << endl;
	cout << 				fieldTwo << endl;
	cout << 			"</h2>" << endl;
	cout << 		"</th>" << endl;	
	cout << 		"<th id = 'fieldThree'>" << endl;
	cout << 			"<h2>" << endl;
	if((fieldThree != "Invalid conversion") && (fieldFour != -1.0))
		cout << fieldThree << endl;
	cout << 			"</h2>" << endl;
	cout << 		"</th>" << endl;
	cout << 		"<th>" << endl;
	cout << 			"<h2 class='blankSpace'>" << endl;
	cout << 				"BLANK" << endl;
	cout << 			"</h2>" << endl;
	cout << 		"</th>" << endl;
	cout << 		"<th id = 'fieldFour'>" << endl;
	cout << 			"<h2>" << endl;
	if((fieldThree == "Invalid conversion") || (fieldFour == -1.0))
		cout << fieldThree << endl;
	else
		cout << fieldFour << endl;
	
	cout << 			"</h2>" << endl;
	cout << 		"</th>" << endl;
	cout << 	"</tr>" << endl;
	cout << "</table>" << endl;
	cout << "</html>" << endl;
}

//prints the HTML document for commodity look up
void printLookupTable(string fieldOne, float fieldTwo, int fieldChecks[]){
	string fieldOneColor = determineColor(fieldChecks[0]);
	string fieldTwoColor = determineColor(fieldChecks[1]);
	
cout << "<!DOCTYPE html>" << endl;
cout << "<html>" << endl;
cout << "<head>" << endl;
cout << 	"<style>" << endl;
cout << 		"table, th {" << endl;
cout << 			"border: 2px solid black;" << endl;
cout << 		"}" << endl;
cout << 		"#fieldOne{" << endl;
cout <<			    "border: 2px solid " << fieldOneColor << endl;
cout << 		"}" << endl;
cout << 		"#fieldTwo{" << endl;
cout << 			"border: 2px solid " << fieldTwoColor << endl;
cout << 		"}" << endl;
cout <<			".blankSpace{" << endl;
cout <<				"color: white " << endl;
cout << 	"</style>" << endl;
cout << "</head>" << endl;
cout << "<table>" << endl;
cout << 	"<tr>" << endl;
cout << 		"<th>" << endl;
cout << 			"<h2>" << endl;
cout << 				"Commodity" << endl;
cout << 			"</h2>" << endl;
cout << 		"</th>" << endl;
cout << 		"<th class='blankSpace'>" << endl;
cout << 			"<h2>" << endl;
cout << 				"BLANK" << endl;
cout << 			"</h2>" << endl;
cout << 		"</th>"	<< endl;
cout << 		"<th>" << endl;
cout << 			"<h2>" << endl;
cout << 				"Result" << endl;
cout << 			"</h2>" << endl;
cout << 		"</th>" << endl;		
cout << 	"</tr>" << endl;
cout << 		"<tr>" << endl;
cout << 		"<th id = 'fieldOne'>" << endl;
cout << 			"<h2>" << endl;
cout << 				fieldOne << endl;
cout << 			"</h2>" << endl;
cout << 		"</th>" << endl;
cout << 		"<th>" << endl;
cout << 			"<h2 class='blankSpace'>" << endl;
cout << 				"BLANK" << endl;
cout << 			"</h2>" << endl;
cout << 		"</th>" << endl;	
cout << 		"<th id = 'fieldTwo'>" << endl;
cout << 			"<h2>" << endl;
cout << 				fieldTwo << endl;
cout << 			"</h2>" << endl;
cout << 		"</th>" << endl;
cout << 	"</tr>" << endl;
cout << "</table>" << endl;
cout << "</html>" << endl;
}

//start of the script
int main(){
	cout << "Content-type:text/html\n\n";
	fflush(stdout);
	
	//seeds the random number generator based on the time of day
	srand(time(NULL));
	
	//fills the dictionary of currency names
	string currencyNames[NUM_CURRENCIES];
	map< string, map<string, float> > exchangeRates;
	fillCurrencies(currencyNames);
	fillExchangeRate(exchangeRates);
	
	//fills the parallel arrays of commodity names, min values, and max max values
	string commodityNames[NUM_COMMODITIES];
	float commodityMinValues[NUM_COMMODITIES];
	float commodityMaxValues[NUM_COMMODITIES];
	
	//fills the commodity dictionaries
	fillCommodities(commodityNames);
	fillCommodityMinValues(commodityMinValues);
	fillCommodityMaxValues(commodityMaxValues);
	
	//initializes the inputs from the GET method
	string inputOneName = "";
	string inputTwoName = "";
	string inputThreeName = "";
	string inputOneValue = "";
	string inputTwoValue = "";
	string inputThreeValue = "";
	
	//parses and stores the provided input from GET
	parseInput(inputOneName, inputTwoName, inputThreeName, inputOneValue, inputTwoValue, inputThreeValue);
	inputOneValue = formatInput(inputOneValue);
	inputTwoValue = formatInput(inputTwoValue);
	
	//used to determine which table needs to be printed
	//initialized to the error table
	int actionIndicator = -1;
	
	//finds the necessary table to be generated
	actionIndicator = determineAction(inputOneName, inputTwoName, inputThreeName);
	
	
	//used to store validation cases for the provided input
	//if the given input is valid holds 0
	//if the given input is empty holds 1
	//if the given input is unknown holds 2
	//if the given input is invalid holds 3
	int fieldChecks[MAX_NUM_FIELDS];
	
	switch(actionIndicator){
		//case in which the currency conversion table is generated
		case 0:
		float convertResult;
		convertResult = convert(exchangeRates, inputOneValue, inputTwoValue, inputThreeValue);
		fieldChecks[0] = checkInput(inputOneValue, currencyNames, NUM_CURRENCIES);
		fieldChecks[1] = checkInput(inputTwoValue, currencyNames, NUM_CURRENCIES);
		fieldChecks[2] = checkInput(inputThreeValue);
		printConvertTable(inputOneValue, inputTwoValue, inputThreeValue, convertResult, fieldChecks);
		break;
		//case in which the commodity look up table is generated
		case 1:
		float lookUpResult;
		lookUpResult = lookup(commodityNames, commodityMinValues, commodityMaxValues, inputOneValue);
		fieldChecks[0] = checkInput(inputOneValue, commodityNames, NUM_COMMODITIES);
		printLookupTable(inputOneValue, lookUpResult, fieldChecks);
		break;
		//case in which the error table is generated
		case -1:
		printConvertTable("", "", "", -1.0, fieldChecks);
		break;
	}
	//END OF SCRIPT
	return 0;
}