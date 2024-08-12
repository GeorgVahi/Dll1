#import "Dll1.dll"
void ShowInputDialog();
double GetText();
#import
double value=0;

void OnTick() {  
   if(MQLInfoInteger(MQL_VISUAL_MODE)) {
       ButtonCreate(0, "target_btn");
       if(ObjectGetInteger(0,"target_btn",OBJPROP_STATE,0) == 1) {
          ShowInputDialog();
          value = GetText();
          ObjectSetString(0,"target_btn",OBJPROP_TEXT,(string)value);
          Print(value); //can use like 'global' #input
       }
   }
}
