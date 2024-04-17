#include <TQObject.h>
#include <RQ_OBJECT.h>

class TGWindow;
class TGMainFrame;
class TRootEmbeddedCanvas;
class TGHorizontalFrame;
class TGCompositeFrame;
class TGTab;
class TGTextButton;
class TGNumberEntry;
class TGLabel;
class TGGroupFrame;
class TGraph;
class TLine;
class TArrow;
class TPaveText;
class TColor;
class TCanvas;
class TTimer;
class TLatex;
class TText;
class TEllipse;

class MyMainFrame {
  RQ_OBJECT("MyMainFrame")
  private:
  TGMainFrame         *MainF;
  TRootEmbeddedCanvas *MCanvas, *MCanvas2;
  TGHorizontalFrame   *HFrame1, *HFrame2, *HFrame3, *DFrame1;
  TGCompositeFrame    *MainCFrame, *SimulF, *ConceptF, *StepsFrame, *GatesFrame;
  TGTab               *SimulTab, *CanvasTab;
  TGTextButton        *TButton1, *TButton2, *TButton3, *next, *previous, *UFGate, *RVal, *DiffGate;
  TGNumberEntry       *NEntry1, *NEntry2, *RValue, *nQubits;
  TGLabel             *Label3, *nLabel;
  TGGroupFrame        *GFrame1, *GFrame2;
  TGraph              *PEstados;  
  TLine               *Circuit1, *Circuit2, *Circuit3, *Circuit4, *Axis1, *Axis2;
  TArrow              *SVector, *SVector2;
  TPaveText           *HGate, *HGate2, *HGate3, *HGate4, *Steps, *UfGate, *DifGate, *MeasureG, *MeasureG2, *MeasureG3, *MeasureG4, *StepGates;
  TColor              *GateColor, *DColor, *UFColor; 
  TCanvas             *fCanvas, *gCanvas;
  TTimer              *Chrono;
  TLatex              *latex;

  double M_state[100];
  int n, N, R, k, r, half;
  double ms[100];
  
public:
  MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
  virtual ~MyMainFrame();
  void Restart();
  void DoDraw();
  void Step1();
  void Oracle();
  void Diffuser();
  void GateConstruct();
  void DifgateDef();
  void RDefinition();
  void Repeat();
  void AnimR();
  void Measure();
};
