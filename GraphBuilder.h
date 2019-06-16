#ifndef _GraphBuilder_GraphBuilder_h_
#define _GraphBuilder_GraphBuilder_h_
#include <SmartUppBot/SmartBotUpp.h>
#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class Discord_DrawPackage: public DiscordModule{
	private:
		//Events discord
		void drawTest(ValueMap payload);
		void DrawGraph(ValueMap payload);
		void testVraiGraph(ValueMap payload);
		
		//Method of class used to generate the graph 
	//	void SaveGraph(graphicalImage &myImage,String fileName="temp.png");
	
		bool isStringisANumber(String stringNumber);
		
	public:
		
		Discord_DrawPackage(Upp::String _name, Upp::String _prefix);
		void Events(ValueMap payload);
	
};


//Virtual Class, supposed to be inherrited to new graph type
enum class DirectionLabel{HAUT=0,BAS=1,GAUCHE=2,DROITE=3};
enum class LabelValueToShow {XVALUE,YVALUE};
enum class ValueTypeEnum{INT,DATE};
class Graph{
	protected:
		String graphName=""; //name of graph 
		String XName=""; //Name of X axis
		String YName=""; //Name of Y axis
		Size sz;
		Font GraphFont = StdFont(GraphFontSize);
		
		int GraphTikeness= 3;
		int GraphFontSize = 20;
		Color MainColor = Black();
		Color AlphaColor = Color(220,220,220);
		
		bool alphaMode = false;
		bool showAxisNames =true;
		bool showGraphName =true;
	public: 
		virtual String ToJson()=0;
		virtual String GetInformation() =0; //Here we force Override in inherited class
											//This func used to print information about the
											//graph
	
		virtual void SetGraphName(Upp::String _GraphName);
		virtual Upp::String GetGraphName();
		
		virtual void DefineSize(Size _sz); //Size value can be acced from size.cx / size.cy
		virtual Size GetSize(); //Return size of Graph (size representing Size of Image would get generated by DrawGraph method)
		
		virtual void DefineXName(String _XName);
		virtual void DefineYName(String _YName);
		virtual String GetXName();
		virtual String GetYName();
		
		virtual void ShowAxisNames(bool b);
		virtual void ShowGraphName(bool b);
		virtual void isAlpha(bool b);
		
		virtual Font GetGraphFont();
		virtual void SetGraphFont(Font _font);
		
		virtual int GetGraphTikeness();
		virtual int GetGraphFontSize();
		virtual void SetGraphTikeness(int _tikeness);
		virtual void SetGraphFontSize(int _FontSize);
		
		virtual Color GetMainColor();
		virtual Color GetAlphaColor();
		virtual void SetMainColor(Color _MainColor);
		virtual void SetAlphaColor(Color _AlphaColor);
		
		//Methode used to draw
		//It have to be used to be AlphaMode Friendly 
		void DrawFlecheAlphaFriendly(Draw& img,int xDebut,int yDebut,int xFin,int yFin,DirectionLabel direction,int tickness=-1,Color color=Color(1,1,1),bool fillWithColor =true,bool AlphaCall =false);
		void DrawTextAlphaFriendly(Draw& img,int xDebut,int yDebut,String TextToDraw="",int angle = 0,Font font=StdFont(1),Color color=Color(1,1,1),bool AlphaCall = false);
		void DrawLineAlphaFriendly(Draw& img, int xDebut,int yDebut,int xFin,int yFin,int tickeness=-1,Color color=Color(1,1,1),bool AlphaCall = false);

};

/***********************************************/
// Oject used to Represent DotCloud graph 
/***********************************************/
class Courbe;
class Dot;

Upp::String ResolveValueTypeEnum(ValueTypeEnum type);

//Not sure if embbed class of Graph into Graph is a good idea
class GraphDotCloud : public Graph, public Upp::Moveable<GraphDotCloud>{
	private:
		const String graphType="DotCloud";
		
		Vector<Courbe> courbes; 
		
		bool showLegendsOfCourbes=false;
		bool showValueOfDot=true;
		bool signIt=true; //This one is here for fun. Signing it with my name ! 
		
		//Translation of value to Axis position
		ValueTypeEnum XValueType =ValueTypeEnum::DATE; //Representing of value Type
		ValueTypeEnum YValueType =ValueTypeEnum::INT; //Representing of value Type
		
		Value xMin;
		Value xMax;
		Value yMin;
		Value yMax;
		bool TranslationDone=false; //This one is to know if StartTranslation has been done (yeah kind of beginners programation)
		bool StartTranslation();
		float ResolveX(Value xToResolve);
		float ResolveY(Value yToResolve);
		
	public:
		String GetTranslationResult();
		String ToJson();
		String GetInformation();
		
		String TypeOfGraph(); //Return type of graph
		bool DrawGraph(); //Used to return an ImageDraw representing the graph
		
		//Data Manipulation
		void AddCourbe(Courbe c);
		void RemoveCourbe(Courbe &c);
		void RemoveCourbe(int i);
		
		Courbe& operator[](int iterator); //allowing to access to courbe stocked in AllCourbes
		
		//Bunch of constructor
		GraphDotCloud(int _XSize,int _YSize);
		GraphDotCloud(int _XSize,int _YSize,String _GraphName);
		GraphDotCloud(int _XSize,int _YSize,String _GraphName, String _XName, String _YName);
		GraphDotCloud(Size _sz,String _GraphName, String _XName, String _YName);
		GraphDotCloud(Size _sz,String _GraphName);
		GraphDotCloud(Size _sz);
		
		//params
		void ShowLegendsOfCourbes(bool b);
		void ShowValueOfDot(bool b);
		void SignIt(bool b);
		
		void DefineXValueType(ValueTypeEnum _xValue);
		void DefineYValueType(ValueTypeEnum _yValue);
		ValueTypeEnum GetXValueType();
		ValueTypeEnum GetYValueType();
		
		
};
 
//this class is here to handle Vector of Dot 
class Courbe : public Upp::Moveable<Courbe>{
	private :
		LabelValueToShow labelToShow = LabelValueToShow::XVALUE; // Define Label to show
		int id; //Here to know Id Of this courbe
		
		String name=""; //Name of the courbe
		Color color=Black(); //Color of the Courbe
		
		ValueTypeEnum XValueType =ValueTypeEnum::DATE; //Representing of value Type
		ValueTypeEnum YValueType =ValueTypeEnum::INT; //Representing of value Type
		
		Vector<Dot> dots; //All dot
	protected:
		bool ShowValueLabel = true; //Show label for each dot
		bool linkDot = true; //protected to allow mother class to access it
	public:
		static int objectCount;//Here to know how many Courbe is On while using graph
		void LinkDot(bool b); //Enable or not linking on dot
		void ValueToShow(LabelValueToShow _labelToShow); //Define axis label to show
		
		Dot& AddDot(Dot d);
		
		Dot& operator[](int iterator);
		void removeDot(int iterator);
		void removeDot(Dot &dot);
		
		String GetInformation();
		int GetId();
		String const GetName() const;
		void SetName(Upp::String _name);
		const Vector<Dot>& GetDots() const;
		
		void SetColor(Color _c);
		Color const GetColor() const;
		
		ValueTypeEnum GetXValueType();
		ValueTypeEnum GetYValueType();
		//Bunch of constructor
		Courbe(const Courbe& c);
		Courbe(String _Name, ValueTypeEnum _XValueType, ValueTypeEnum _YValueType,Color _color);
		Courbe(String _Name, ValueTypeEnum _XValueType, ValueTypeEnum _YValueType);
		Courbe(String _Name);
		
		~Courbe();
};

class Dot : public Upp::Moveable<Dot>{
	private:
		int id;
		Courbe * parent;
		Value XValue;
		Value YValue;		
	public:
		static int objectCount;
		Value GetXVal();
		void SetXValue(Value _XValue);
		
		Value GetYVal();
		void SetYValue(Value _YValue);
		
		String GetInformation();
		int GetId();
		
		Dot(Value _XValue,Value _YValue,Courbe* _parent);
		~Dot();
};




/***********************************************/
// End of Courbe graph
/***********************************************/

Graph* ConstructGraphFromJSON(ValueMap &json); //This is supposed to return graph object from a complete Json
		


#endif
