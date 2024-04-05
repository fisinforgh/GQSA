#include <TApplication.h>
#include <TGClient.h>
#include <TCanvas.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TMath.h>
#include <TRootEmbeddedCanvas.h>
#include <TGraph.h>
#include <TGTab.h>
#include <TTimer.h>
#include <TText.h>
#include <TGNumberEntry.h>
#include <TGLabel.h>
#include <TLine.h>
#include <TLatex.h>
#include <TArrow.h>
#include <TPaveText.h>
#include <TColor.h>
#include <TGIcon.h>
#include <TEllipse.h>
#include <TMatrixT.h>
#include <TROOT.h>
#include <iostream>
#include <cmath>
#include "InterfaceGQSA.h"

MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) {
   // Create a main frame
   MainF = new TGMainFrame(p,w,h);
   MainF->SetWMSizeHints(w,h,2*w,2*h,10,10);
   MainF->SetWindowName("Grover's Algorithm");


   HFrame1 = new TGHorizontalFrame(MainF);
   DFrame1 = new TGHorizontalFrame(MainF);
   HFrame2 = new TGHorizontalFrame(DFrame1);
   HFrame3 = new TGHorizontalFrame(DFrame1);
   MainCFrame = new TGCompositeFrame(HFrame1, kVerticalFrame);
   SimulTab = new TGTab(MainCFrame);
   CanvasTab = new TGTab(HFrame1);
   
   SimulF = SimulTab->AddTab("Simulation");
   SimulF->SetLayoutManager(new TGVerticalLayout(SimulF));
   ConceptF = SimulTab->AddTab("Basic Concepts");
   ConceptF->SetLayoutManager(new TGVerticalLayout(ConceptF));

   StepsFrame = CanvasTab->AddTab("Steps");
   StepsFrame->SetLayoutManager(new TGHorizontalLayout(StepsFrame));
   GatesFrame = CanvasTab->AddTab("Gates");
   GatesFrame->SetLayoutManager(new TGHorizontalLayout(GatesFrame));

   MCanvas = new TRootEmbeddedCanvas("Lienzo",StepsFrame,w,h);

   GFrame1 = new TGGroupFrame(SimulF,"Number of Elements (N)");
   GFrame2 = new TGGroupFrame(SimulF, "Search Element");

   TButton1 = new TGTextButton (HFrame2, "Start");
   TButton1->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
   TButton2 = new TGTextButton (HFrame2, "Restart");
   TButton2->Connect("Clicked()", "MyMainFrame", this, "Restart()");
   TButton3 = new TGTextButton (HFrame2, "Close", "gApplication->Terminate(0)");
   next = new TGTextButton (HFrame2, "Next");
   previous = new TGTextButton (HFrame2, "Previous");

   UFGate = new TGTextButton(ConceptF, "Uf Gate");
   UFGate->Connect("Clicked()", "MyMainFrame", this, "GateConstruct()");
   RVal = new TGTextButton(ConceptF, "R Value");
   RVal->Connect("Clicked()", "MyMainFrame", this, "RDefinition()");
   DiffGate = new TGTextButton(ConceptF, "Diffuser Gate");
   DiffGate->Connect("Clicked()", "MyMainFrame", this, "DifgateDef()");
  
   NEntry1 = new TGNumberEntry(GFrame1);
   NEntry1->SetLimits(TGNumberFormat::kNELLimitMinMax,3,64);
   
   NEntry2 = new TGNumberEntry(GFrame2);
   NEntry2->SetLimits(TGNumberFormat::kNELLimitMinMax,1,64);
   
   Label3 = new TGLabel(SimulF, "R Value:");

   nLabel = new TGLabel(SimulF, "Number of qubits (n):");

   RValue = new TGNumberEntry(SimulF);
   RValue->SetState(kFALSE);

   nQubits = new TGNumberEntry(SimulF);
   nQubits->SetState(kFALSE);

   
   TGLayoutHints *Centrar = new TGLayoutHints (kLHintsCenterX | kLHintsCenterY ,5 ,5 ,5 ,5);
   TGLayoutHints *Expandir = new TGLayoutHints (kLHintsExpandX | kLHintsExpandY ,10 ,10 ,10 ,10);
   TGLayoutHints *CentrarX = new TGLayoutHints (kLHintsCenterX ,5 ,5);
   TGLayoutHints *CentrarY = new TGLayoutHints (kLHintsCenterY ,5 ,5);
   TGLayoutHints *ExpandirX = new TGLayoutHints (kLHintsExpandX ,0 ,10);
   TGLayoutHints *ExpandirY = new TGLayoutHints (kLHintsExpandY ,10 ,10);

   TGIcon *IconUD= new TGIcon(HFrame3, "pictures/LogoUD1.png");
   TGIcon *IFisinfor = new TGIcon(HFrame3, "pictures/logo_fisinfor.png");
   TGIcon *GUIIcon = new TGIcon(DFrame1, "pictures/LogoGUI.png");

   GFrame1->AddFrame(NEntry1,Centrar);
   GFrame2->AddFrame(NEntry2,Centrar);

   SimulF->AddFrame(GFrame1,CentrarX);
   SimulF->AddFrame(GFrame2,CentrarX);
   SimulF->AddFrame(Label3, new TGLayoutHints(kLHintsCenterX, 5,5,10,5));
   SimulF->AddFrame(RValue,CentrarX);
   SimulF->AddFrame(nLabel, new TGLayoutHints(kLHintsCenterX, 5,5,25,5));
   SimulF->AddFrame(nQubits, new TGLayoutHints(kLHintsCenterX, 5,5,5,5));

   ConceptF->AddFrame(UFGate, new TGLayoutHints(kLHintsCenterX, 20,5,5,20));
   ConceptF->AddFrame(DiffGate, new TGLayoutHints(kLHintsCenterX, 20,5,5,20));  
   ConceptF->AddFrame(RVal, new TGLayoutHints(kLHintsCenterX, 20,5,5,20));

   HFrame2->AddFrame(TButton1,new TGLayoutHints(kLHintsCenterY | kLHintsCenterX, 5,5,5,5));
   HFrame2->AddFrame(TButton2,Centrar);
   HFrame2->AddFrame(TButton3,Centrar);
   HFrame2->AddFrame(previous, Centrar);
   HFrame2->AddFrame(next, Centrar);

   
   HFrame3->AddFrame(IFisinfor, new TGLayoutHints(kLHintsCenterY, 2,2,2,2));
   IFisinfor->Resize(70,85);
   HFrame3->AddFrame(IconUD, new TGLayoutHints(kLHintsCenterY, 2,2,2,2));
   IconUD->Resize(70,80);
   DFrame1->AddFrame(GUIIcon, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 5,5,5,5));
   GUIIcon->Resize(150,75);
   
   MainCFrame->AddFrame(SimulTab,ExpandirY);

   HFrame1->AddFrame(MainCFrame, ExpandirY);
   HFrame1->AddFrame(CanvasTab, Expandir);
   StepsFrame->AddFrame(MCanvas, Expandir);
   
   DFrame1->AddFrame(HFrame2,new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 2,2,2,2));
   DFrame1->AddFrame(HFrame3,new TGLayoutHints(kLHintsRight, 2,2,0,0));

   MainF->AddFrame(HFrame1,Expandir);
   MainF->AddFrame(DFrame1,ExpandirX);

   
   TText *t = new TText(0.5,0.97,"SE GROVER'S QUANTUM SEARCH ALGORITHM");
   t->SetTextAlign(22);
   t->SetTextColor(kRed+2);
   t->SetTextFont(22);
   t->SetTextSize(0.05);
   t->Draw();
   
   TPaveText *Welcome = new TPaveText(0.01,0.59,0.99,0.93);
   Welcome->SetTextAlign(13);
   Welcome->SetTextFont(42);
   Welcome->SetFillColor(kOrange-4);
   Welcome->SetTextSize(0.03);
   Welcome->AddText("Welcome to the GQSA software of education, this is a Graphical User Interface (GUI) for the teaching and learning of Grover's Algorithm,");
   Welcome->AddText("in this GUI you will have a deep look into the algorithm getting to know in detail different information concerning the algorithm,");
    Welcome->AddText("including: the gates construction, the R value and definition; aswell you would find a simulation section, in which it is possible to");
   Welcome->AddText(" visualize the algorithm step by step and watch the volution of the states.");
   Welcome->AddText("In the simulation the circuit, the steps of the algorithm and matrices of the gates, the geometric visualization, and the amplitude of");
   Welcome->AddText("the states are shown. Being able to take an step by step look, and understand the progression and  changes in the algorithm as the");
   Welcome->AddText("parameters are changed.");
   Welcome->AddText("We recommend to check first the basic concepts tab and then, for the simulation, to start with few amount of elements, and progresively increase");
   Welcome->AddText("them in order to understand better the algorithm and the changes in it.");
   Welcome->AddText("");
   Welcome->Draw();
   
   TPaveText *PaveInfo = new TPaveText(0.01,0.01,0.35,0.57);
   PaveInfo->SetTextAlign(13);
   PaveInfo->SetTextFont(42);
   PaveInfo->SetFillColor(kBlue-10);
   PaveInfo->SetTextSize(0.03);
   PaveInfo->AddText("The Grover's algorithm is a quantum computing");
   PaveInfo->AddText("algorithm for the search of an element in a");
   PaveInfo->AddText("disordered list, this algorithm has a better");
   PaveInfo->AddText("performance than a classic one, where the");
   PaveInfo->AddText("avarage amount of iterations needed is #alpha N.");
   PaveInfo->AddText("While Grover's algorithm finds the desired");
   PaveInfo->AddText("element in #alpha#sqrt{N} iterations where N");
   PaveInfo->AddText("is the total amount of elements in the list.");
   PaveInfo->AddText("Grover's algortihm is one of the most mention");
   PaveInfo->AddText("quantum algorithms in the literature.");
   PaveInfo->Draw();

   TPaveText *PaveT1 = new TPaveText(0.36,0.01,0.715,0.57);
   PaveT1->SetTextAlign(13);
   PaveT1->SetTextFont(131);
   PaveT1->SetTextSize(0.03);
   PaveT1->SetTextColor(0);
   PaveT1->SetFillColor(kAzure+1);
   PaveT1->AddText("On the left you will find two tabs, the first one for");
   PaveT1->AddText("the simulation of the algorithm, this contains number");
   PaveT1->AddText("entries, which you can interact with, these belong to");
   PaveT1->AddText("the number of elements (N) to search and the search");
   PaveT1->AddText("element, both of those can be changed as the user need,");
   PaveT1->AddText("it below them, there is the R value and the number of");
   PaveT1->AddText("qubits (n), these values will be update when the");
   PaveT1->AddText("simulation begins. On the other tab you will find");
   PaveT1->AddText("explanations of important concepts for the algorithm");
   PaveT1->AddText("At the bottom you will find 5 buttons: the Start use to");
   PaveT1->AddText("begin the simulation, the Restart for begining a new");
   PaveT1->AddText("simulation, the Close for exiting the program, and the");
   PaveT1->AddText("next and previous buttons for navigate among the");
   PaveT1->AddText("different steps of the algorithm.");
   PaveT1->AddText("");
   PaveT1->Draw();
  

   TPaveText *pavecred = new TPaveText(0.725,0.01,0.99,0.57);
   pavecred->SetTextAlign(10);
   pavecred->SetTextFont(42);
   pavecred->SetTextColor(0);
   pavecred->SetTextSize(0.025);
   pavecred->SetFillColor(kBlue-6);
   
   pavecred->AddText(" ");
   pavecred->AddText(" Authors: ");
   pavecred->AddText(" #font[62]{Daniel M Martin R}");
   pavecred->AddText(" FISINFOR research group");
   pavecred->AddText(" #font[62]{Julian A Salamanca B}, PHD ");
   pavecred->AddText(" FISINFOR research group leader ");
   pavecred->AddText(" SCIENCE AND EDUCATION FACULTY");
   pavecred->AddText(" Professor #font[72]{UNIVERSIDAD DISTRITAL}");
   pavecred->AddText(" #font[72]{FRANCISCO JOSE DE CALDAS}");

   pavecred->Draw();

   MCanvas2 = new TRootEmbeddedCanvas("Lienzo2",GatesFrame,w,h);
   GatesFrame->AddFrame(MCanvas2, Expandir);

   t->Draw();
   Welcome->Draw();
   PaveInfo->Draw();
   PaveT1->Draw();
   pavecred->Draw();
   
   MainF->MapSubwindows();
   MainF->Resize();
   MainF->MapWindow();

   
}

void MyMainFrame::Restart(){

  fCanvas = MCanvas->GetCanvas();
  fCanvas->Clear();

  TButton1->SetState(kButtonUp, kTRUE);
  NEntry1->SetState(kTRUE);
  NEntry2->SetState(kTRUE);
  
  TText *t = new TText(0.5,0.97,"SE GROVER'S QUANTUM SEARCH ALGORITHM");
  t->SetTextAlign(22);
  t->SetTextColor(kRed+2);
  t->SetTextFont(22);
  t->SetTextSize(0.05);
  t->Draw();
  
  TPaveText *Welcome = new TPaveText(0.01,0.59,0.99,0.93);
  Welcome->SetTextAlign(13);
  Welcome->SetTextFont(42);
  Welcome->SetFillColor(kOrange-4);
  Welcome->SetTextSize(0.03);
  Welcome->AddText("Welcome to the GQSA software of education, this is a Graphical User Interface (GUI) for the teaching and learning of Grover's Algorithm,");
  Welcome->AddText("in this GUI you will have a deep look into the algorithm getting to know in detail different information concerning the algorithm,");
  Welcome->AddText("including: the gates construction, the R value and definition; aswell you would find a simulation section, in which it is possible to");
  Welcome->AddText(" visualize the algorithm step by step and watch the volution of the states.");
  Welcome->AddText("In the simulation the circuit, the steps of the algorithm and matrices of the gates, the geometric visualization, and the amplitude of");
  Welcome->AddText("the states are shown. Being able to take an step by step look, and understand the progression and  changes in the algorithm as the");
  Welcome->AddText("parameters are changed.");
  Welcome->AddText("");
  Welcome->Draw();
  
  TPaveText *PaveInfo = new TPaveText(0.01,0.01,0.35,0.57);
  PaveInfo->SetTextAlign(13);
  PaveInfo->SetTextFont(42);
  PaveInfo->SetFillColor(kBlue-10);
  PaveInfo->SetTextSize(0.03);
  PaveInfo->AddText("The Grover's algorithm is a quantum computing");
  PaveInfo->AddText("algorithm for the search of an element in a");
  PaveInfo->AddText("disordered list, this algorithm has a better");
  PaveInfo->AddText("performance than a classic one, where the");
  PaveInfo->AddText("avarage amount of iterations needed is #alpha N.");
  PaveInfo->AddText("While Grover's algorithm finds the desired");
  PaveInfo->AddText("element in #alpha#sqrt{N} iterations where N");
  PaveInfo->AddText("is the total amount of elements in the list.");
  PaveInfo->AddText("Grover's algortihm is one of the most mention");
  PaveInfo->AddText("quantum algorithms in the literature.");
  PaveInfo->Draw();
  
  TPaveText *PaveT1 = new TPaveText(0.36,0.01,0.715,0.57);
  PaveT1->SetTextAlign(13);
  PaveT1->SetTextFont(131);
  PaveT1->SetTextSize(0.03);
  PaveT1->SetTextColor(0);
  PaveT1->SetFillColor(kAzure+1);
  PaveT1->AddText("On the left you will find two tabs, the first one for");
  PaveT1->AddText("the simulation of the algorithm, this contains number");
  PaveT1->AddText("entries, which you can interact with, these belong to");
  PaveT1->AddText("the number of elements (N) to search and the search");
  PaveT1->AddText("element, both of those can be changed as the user need,");
  PaveT1->AddText("it below them, there is the R value and the number of");
  PaveT1->AddText("qubits (n), these values will be update when the");
  PaveT1->AddText("simulation begins. On the other tab you will find");
  PaveT1->AddText("explanations of important concepts for the algorithm");
  PaveT1->AddText("At the bottom you will find 5 buttons: the Start use to");
  PaveT1->AddText("begin the simulation, the Restart for begining a new");
  PaveT1->AddText("simulation, the Close for exiting the program, and the");
  PaveT1->AddText("next and previous buttons for navigate among the");
  PaveT1->AddText("different steps of the algorithm.");
  PaveT1->AddText("");
  PaveT1->Draw();
  
  
  TPaveText *pavecred = new TPaveText(0.725,0.01,0.99,0.57);
  pavecred->SetTextAlign(10);
  pavecred->SetTextFont(42);
  pavecred->SetTextColor(0);
  pavecred->SetTextSize(0.025);
  pavecred->SetFillColor(kBlue-6);
  
  pavecred->AddText(" ");
  pavecred->AddText(" Authors: ");
  pavecred->AddText(" #font[62]{Daniel M Martin R}");
  pavecred->AddText(" FISINFOR research group");
  pavecred->AddText(" #font[62]{Julian A Salamanca B}, PHD ");
  pavecred->AddText(" FISINFOR research group leader ");
  pavecred->AddText(" SCIENCE AND EDUCATION FACULTY");
  pavecred->AddText(" Professor #font[72]{UNIVERSIDAD DISTRITAL}");
  pavecred->AddText(" #font[72]{FRANCISCO JOSE DE CALDAS}");
  
  pavecred->Draw();
   
  fCanvas->Update();
  
  fCanvas = MCanvas2->GetCanvas();
  fCanvas->Clear();
  
  t->Draw();
  Welcome->Draw();
  PaveInfo->Draw();
  PaveT1->Draw();
  pavecred->Draw();
  
  fCanvas->Update();
  
}


void MyMainFrame::DoDraw() {
  
  fCanvas = MCanvas->GetCanvas();
  fCanvas->Clear();
  fCanvas->Update();
  fCanvas->Divide(2,2);

  gCanvas = MCanvas2->GetCanvas();
  gCanvas->Clear();
  gCanvas->Update();
  gCanvas->Divide(2,2);
  
  TButton1->SetState(kButtonDisabled, kTRUE);
  NEntry1->SetState(kFALSE);
  NEntry2->SetState(kFALSE);
  
  TText *C = new TText(0.5,0.95,"Circuit");
  C->SetTextAlign(22);
  C->SetTextColor(kBlue+2);
  C->SetTextFont(22);
  C->SetTextSize(0.09);
  
  TText *GV = new TText(-0.57,0.9, "Geometric Visualization");
  GV->SetTextAlign(22);
  GV->SetTextColor(kBlue+2);
  GV->SetTextFont(22);
  GV->SetTextSize(0.08);

  latex = new TLatex();
  
  
  N = NEntry1->GetNumber();
  k = NEntry2->GetNumber();
  n = ceil(log2(N));

  R = round((sqrt(N)*TMath::Pi()/4.)-0.5);

  RValue->TGNumberEntry::GetButtonUp();

  RValue->SetNumber(R);

  nQubits->TGNumberEntry::GetButtonUp();
  
  nQubits->SetNumber(n);
  
  
  double s[N];

  for(int i=0; i<=N; i++){
    
    s[i]=i+1;
  }
  //ms[N];

  for(int i=0; i<=N; i++){

    ms[i]=0;
  }
  
  ms[0]=1;
  
  TMatrixT<double> State(N,1,s);
  //State.Print();
  
  PEstados = new TGraph(N,s,ms);
  PEstados->SetFillColor(40);
  PEstados->SetTitle("States Amplitude; Elements; Amplitude");
  
  Steps = new TPaveText(0.2,0.1,0.8,0.9);
  Steps->SetTextAlign(12);
  Steps->SetShadowColor(0);
  Steps->SetFillColor(0);
  Steps->SetLineColor(0);
  Steps->SetTextSize(0.05);
  Steps->AddText("Initialize the n qubits to state |0#GT");((TText*)Steps->GetListOfLines()->Last())->SetTextColor(kOrange+1);
  Steps->AddText("Apply Hadamard Gate over the n qubits:");
  Steps->AddText("H^{#otimesn}|0#GT^{#otimesn}");
  Steps->AddText("Apply a sign shift with the oracle U_{f} ");
  Steps->AddText("Apply the Diffusion transform D");
  Steps->AddText("Measure the states (qubits)");

  StepGates = new TPaveText(0.2,0.1,0.8,0.9);
  StepGates->SetTextAlign(12);
  StepGates->SetShadowColor(0);
  StepGates->SetFillColor(0);
  StepGates->SetLineColor(0);
  StepGates->SetTextSize(0.05);
  StepGates->AddText("Initialize the n qubits to state |0#GT");
  
  
  if(k>N){
    std::cout<<"Number of Elements must be greater than Search Element"<<std::endl;
    Restart();
  }
    
  switch(n){
  case 2:
    fCanvas->cd(1);
    
    Circuit1 = new TLine(0.06,0.7,0.99,0.7);
    Circuit2 = new TLine(0.06,0.3,0.99,0.3);
    
    
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.685,"|0#GT");
    latex->DrawLatex(0.02,0.285,"|0#GT");
    
    C->Draw();
    
    Circuit1->Draw();
    Circuit2->Draw();

    gCanvas->cd(1);

    latex->DrawLatex(0.02,0.685,"|0#GT");
    latex->DrawLatex(0.02,0.285,"|0#GT");
    C->Draw();
    
    Circuit1->Draw();
    Circuit2->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();

    gCanvas->cd(2);

    StepGates->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
        
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");

    gCanvas->cd(4);
    
    PEstados->Draw("AB");
     
    fCanvas->Update();
    gCanvas->Update();
    
    break;
  case 3:
    
    fCanvas->cd(1);
    
    Circuit1 = new TLine(0.06,0.75,0.99,0.75);
    Circuit2 = new TLine(0.06,0.5,0.99,0.5);
    Circuit3 = new TLine(0.06,0.25,0.99,0.25);
    
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.485,"|0#GT");
    latex->DrawLatex(0.02,0.235,"|0#GT");
    
    C->Draw();
    
    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();

    gCanvas->cd(1);

    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.485,"|0#GT");
    latex->DrawLatex(0.02,0.235,"|0#GT");
    
    C->Draw();
    
    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();

    gCanvas->cd(2);

    StepGates->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
        
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");

    gCanvas->cd(4);

    PEstados->Draw("AB");
     
    fCanvas->Update();
    gCanvas->Update();
    
    break;
  case 4:

    fCanvas->cd(1);
    
    Circuit1 = new TLine(0.06,0.75,0.99,0.75);
    Circuit2 = new TLine(0.06,0.55,0.99,0.55);
    Circuit3 = new TLine(0.06,0.35,0.99,0.35);
    Circuit4 = new TLine(0.06,0.15,0.99,0.15);
    
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.535,"|0#GT");
    latex->DrawLatex(0.02,0.335,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");
    
    C->Draw();
    
    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();
    Circuit4->Draw();

    gCanvas->cd(1);

    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.535,"|0#GT");
    latex->DrawLatex(0.02,0.335,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");
    
    C->Draw();
    
    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();
    Circuit4->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();

    gCanvas->cd(2);

    StepGates->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
        
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");

    gCanvas->cd(4);

    PEstados->Draw("AB");
    
    fCanvas->Update();
    gCanvas->Update();
    
    break;
  }

  if(n>4){

    fCanvas->cd(1)->SetPad(0.01,0.51,0.85,0.99);
    
    Circuit1 = new TLine(0.06,0.75,0.99,0.75);
    Circuit2 = new TLine(0.06,0.15,0.99,0.15);
    
    
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");
    latex->SetTextSize(0.2);
    latex->DrawLatex(0.03,0.56,"#upoint");
    latex->DrawLatex(0.03,0.41,"#upoint");
    latex->DrawLatex(0.03,0.26,"#upoint");
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.0,0.45,"n");
    
    C->Draw();
    
    Circuit1->Draw();
    Circuit2->Draw();

    gCanvas->cd(1)->SetPad(0.01,0.51,0.85,0.99);

    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");
    latex->SetTextSize(0.2);
    latex->DrawLatex(0.03,0.56,"#upoint");
    latex->DrawLatex(0.03,0.41,"#upoint");
    latex->DrawLatex(0.03,0.26,"#upoint");
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.0,0.45,"n");
    
    C->Draw();
    
    Circuit1->Draw();
    Circuit2->Draw();
    
    fCanvas->cd(2)->SetPad(0.86,0.51,0.99,0.99);
    gCanvas->cd(2)->SetPad(0.86,0.51,0.99,0.99);
    
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");

    gCanvas->cd(4);
    PEstados->Draw("AB");
     
    fCanvas->Update();
    gCanvas->Update();

  }

  next->Disconnect("Clicked()");
  next->Connect("Clicked()","MyMainFrame",this,"Step1()");
}
//DoDraw End

//.............................................................................

// Step 1 Begin

void MyMainFrame::Step1(){

  fCanvas->Clear();
  fCanvas->Divide(2,2);
  fCanvas->Update();
  gCanvas->Clear();
  gCanvas->Update();
  gCanvas->Divide(2,2);
  GateColor=gROOT->GetColor(20);
  GateColor->SetRGB(0.22,0.33,0.61);

  TText *C = new TText(0.5,0.95,"Circuit");
  C->SetTextAlign(22);
  C->SetTextColor(kBlue+2);
  C->SetTextFont(22);
  C->SetTextSize(0.09);
  
  TText *GV = new TText(-0.57,0.9, "Geometric Visualization");
  GV->SetTextAlign(22);
  GV->SetTextColor(kBlue+2);
  GV->SetTextFont(22);
  GV->SetTextSize(0.08);

  latex = new TLatex();

  ms[N];
  
  for(int i=0; i<=N; i++){
    
    ms[i]=1/sqrt(N);
  }
  
  double s[N];
  
  for(int i=0; i<=N; i++){
    
    s[i]=i+1;
  }
  
  PEstados = new TGraph(N,s,ms);
  PEstados->SetFillColor(40);
  PEstados->SetTitle("States Amplitude; Elements; Amplitude");

  Steps = new TPaveText(0.2,0.1,0.8,0.9);
  Steps->SetTextAlign(12);
  Steps->SetShadowColor(0);
  Steps->SetFillColor(0);
  Steps->SetLineColor(0);
  Steps->SetTextSize(0.05);
  Steps->AddText("Initialize the n qubits to state |0#GT"); 
  Steps->AddText("Apply Hadamard Gate over the n qubits:");((TText*)Steps->GetListOfLines()->Last())->SetTextColor(kOrange+1);
  Steps->AddText("H^{#otimesn}|0#GT^{#otimesn}");((TText*)Steps->GetListOfLines()->Last())->SetTextColor(kOrange+1);
  Steps->AddText("Apply a sign shift with the oracle U_{f} ");
  Steps->AddText("Apply the Diffusion transform D");
  Steps->AddText("Measure the states (qubits)");

  StepGates = new TPaveText(0.2,0.1,0.8,0.9);
  StepGates->SetTextAlign(12);
  StepGates->SetShadowColor(0);
  StepGates->SetFillColor(0);
  StepGates->SetLineColor(0);
  StepGates->SetTextSize(0.05);
  StepGates->AddText("Apply Hadamard Gate:");
  StepGates->AddText("#color[20]{   H = #frac{1}{#sqrt{2}} #scale[2.5]{(} #splitline{1}{1}    #splitline{ 1}{-1} #scale[2.5]{)}}");
  StepGates->AddText("The Hadamard Gate produces an equal superposition state");
  StepGates->AddText("");

  double xf=sqrt((N-1)*(1./N));
  double yf=sqrt(1./N);
  SVector = new TArrow(0.0,0.0,xf,yf,0.015,"|>");
  SVector->SetFillColor(2);
  SVector->SetLineColor(2);
  
  switch(n){
  case 2:
    fCanvas->cd(1);
    Circuit1 = new TLine(0.06,0.7,0.99,0.7);
    Circuit2 = new TLine(0.06,0.3,0.99,0.3);
    
    HGate = new TPaveText(0.08,0.624,0.16,0.774);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");
    
    HGate2 = new TPaveText(0.08,0.224,0.16,0.374);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");
    
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.685,"|0#GT");
    latex->DrawLatex(0.02,0.285,"|0#GT");
    
    Circuit1->Draw();
    Circuit2->Draw();
    HGate->Draw();
    HGate2->Draw();
    
    C->Draw();

    gCanvas->cd(1);

    latex->DrawLatex(0.02,0.685,"|0#GT");
    latex->DrawLatex(0.02,0.285,"|0#GT");
    
    Circuit1->Draw();
    Circuit2->Draw();
    HGate->Draw();
    HGate2->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();

    gCanvas->cd(2);

    StepGates->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.09,yf+0.06,"|#varphi#GT"); 
     
    Axis1->Draw();
    Axis2->Draw();
    SVector->Draw();
    GV->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.09,yf+0.06,"|#varphi#GT"); 
     
    Axis1->Draw();
    Axis2->Draw();
    SVector->Draw();
    GV->Draw();
    
    fCanvas->cd(4);
    
    PEstados->Draw("AB");

    gCanvas->cd(4);

    PEstados->Draw("AB");
    
    fCanvas->Update();
    gCanvas->Update();
    
    break;
  case 3:
    
    fCanvas->cd(1);
    
    C->Draw();
    
    Circuit1 = new TLine(0.06,0.75,0.99,0.75);
    Circuit2 = new TLine(0.06,0.5,0.99,0.5);
    Circuit3 = new TLine(0.06,0.25,0.99,0.25);
    
    HGate = new TPaveText(0.08,0.674,0.16,0.824);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");
    
    HGate2 = new TPaveText(0.08,0.424,0.16,0.574);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");

    HGate3 = new TPaveText(0.08,0.174,0.16,0.324);
    HGate3->SetTextAlign(22);
    HGate3->SetShadowColor(0);
    HGate3->SetTextFont(11);
    HGate3->SetTextSize(0.07);
    HGate3->SetFillColor(20);
    HGate3->AddText("H");

    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.485,"|0#GT");
    latex->DrawLatex(0.02,0.235,"|0#GT");

    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();

    HGate->Draw();
    HGate2->Draw();
    HGate3->Draw();

    gCanvas->cd(1);

    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.485,"|0#GT");
    latex->DrawLatex(0.02,0.235,"|0#GT");

    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();

    HGate->Draw();
    HGate2->Draw();
    HGate3->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();

    gCanvas->cd(2);
    
    StepGates->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.09,yf+0.06,"|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.09,yf+0.06,"|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");

    gCanvas->cd(4);
    PEstados->Draw("AB");
     
    fCanvas->Update();
    gCanvas->Update();
    
     break;
  case 4:
    fCanvas->cd(1);
    
    C->Draw();
    
    Circuit1 = new TLine(0.06,0.75,0.99,0.75);
    Circuit2 = new TLine(0.06,0.55,0.99,0.55);
    Circuit3 = new TLine(0.06,0.35,0.99,0.35);
    Circuit4 = new TLine(0.06,0.15,0.99,0.15);
    
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.535,"|0#GT");
    latex->DrawLatex(0.02,0.335,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");
    
    HGate = new TPaveText(0.08,0.674,0.16,0.824);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");
    
    HGate2 = new TPaveText(0.08,0.474,0.16,0.624);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");

    HGate3 = new TPaveText(0.08,0.274,0.16,0.424);
    HGate3->SetTextAlign(22);
    HGate3->SetShadowColor(0);
    HGate3->SetTextFont(11);
    HGate3->SetTextSize(0.07);
    HGate3->SetFillColor(20);
    HGate3->AddText("H");

    HGate4 = new TPaveText(0.08,0.074,0.16,0.224);
    HGate4->SetTextAlign(22);
    HGate4->SetShadowColor(0);
    HGate4->SetTextFont(11);
    HGate4->SetTextSize(0.07);
    HGate4->SetFillColor(20);
    HGate4->AddText("H");

    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();
    Circuit4->Draw();

    HGate->Draw();
    HGate2->Draw();
    HGate3->Draw();
    HGate4->Draw();

    gCanvas->cd(1);

    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.535,"|0#GT");
    latex->DrawLatex(0.02,0.335,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");

    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();
    Circuit4->Draw();

    HGate->Draw();
    HGate2->Draw();
    HGate3->Draw();
    HGate4->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();

    gCanvas->cd(2);
    
    StepGates->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.09,yf+0.06,"|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.09,yf+0.06,"|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");

    gCanvas->cd(4);
   
    PEstados->Draw("AB");

    fCanvas->Update();
    gCanvas->Update();
    
     break;
  }

  if(n>4){

    fCanvas->cd(1)->SetPad(0.01,0.51,0.85,0.99);
    
    C->Draw();
    
    Circuit1 = new TLine(0.06,0.75,0.99,0.75);
    Circuit2 = new TLine(0.06,0.15,0.99,0.15);
    
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");
    latex->SetTextSize(0.2);
    latex->DrawLatex(0.03,0.56,"#upoint");
    latex->DrawLatex(0.03,0.41,"#upoint");
    latex->DrawLatex(0.03,0.26,"#upoint");
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.0,0.45,"n");
    
    HGate = new TPaveText(0.08,0.674,0.125,0.824);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");

    HGate2 = new TPaveText(0.08,0.074,0.125,0.224);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");

    Circuit1->Draw();
    Circuit2->Draw();

    HGate->Draw();
    HGate2->Draw();

    gCanvas->cd(1)->SetPad(0.01,0.51,0.85,0.99);

    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");
    latex->SetTextSize(0.2);
    latex->DrawLatex(0.03,0.56,"#upoint");
    latex->DrawLatex(0.03,0.41,"#upoint");
    latex->DrawLatex(0.03,0.26,"#upoint");
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.0,0.45,"n");

    C->Draw();
    Circuit1->Draw();
    Circuit2->Draw();

    HGate->Draw();
    HGate2->Draw();
    
    
    fCanvas->cd(2)->SetPad(0.86,0.51,0.99,0.99);
    gCanvas->cd(2)->SetPad(0.86,0.51,0.99,0.99);
    
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.09,yf+0.06,"|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.09,yf+0.06,"|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");

    gCanvas->cd(4);

    PEstados->Draw("AB");
    
    fCanvas->Update();
    gCanvas->Update();

  }

  next->Disconnect("Clicked()");
  previous->Disconnect("Clicked()");
  next->Connect("Clicked()","MyMainFrame",this,"Oracle()");
  previous->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
}
// End Step 1

//..............................................................................

//Begin Oracle

void MyMainFrame::Oracle(){
  
  fCanvas->Clear();
  fCanvas->Update();
  fCanvas->Divide(2,2);
  gCanvas->Clear();
  gCanvas->Update();
  gCanvas->Divide(2,2);
  GateColor=gROOT->GetColor(20);
  UFColor=gROOT->GetColor(22);
  GateColor->SetRGB(0.22,0.33,0.61);
  UFColor->SetRGB(0.58,0.47,0.90);
  
  TText *C = new TText(0.5,0.95,"Circuit");
  C->SetTextAlign(22);
  C->SetTextColor(kBlue+2);
  C->SetTextFont(22);
  C->SetTextSize(0.09);
  
  TText *GV = new TText(-0.57,0.9, "Geometric Visualization");
  GV->SetTextAlign(22);
  GV->SetTextColor(kBlue+2);
  GV->SetTextFont(22);
  GV->SetTextSize(0.08);

  latex = new TLatex();

  for(int i=0; i<=N; i++){
    
    ms[i]=1/sqrt(N);
  }
  
  double uf[N*N];
  
  for(int i=0; i<=(N*N); i++){
    uf[i]=0.0;
    
    //std::cout<<uf[0]<<std::endl;
  }
  
  for(int i=1; i<=N; i++){
    
    int x = (i-1)+(N*(i-1)); 
    
    uf[x]=1;
    
  }
  
  int s_e = (k-1)+(N*(k-1));
  
  uf[s_e]=-1;
  
  TMatrixT<double> Uf(N,N,uf);

  double s[N];
  
  for(int i=0; i<=N; i++){
    
    s[i]=i+1;
  }

  TMatrixT<double> State(N,1,ms);

  TMatrixT<double> OResult = Uf*State;
  
  double *Res = OResult.GetMatrixArray();

  for(int i=0; i<=N; i++){
    
    M_state[i]=Res[i];
  }
  
  for(int i=0; i<=N; i++){
    
    ms[i]=Res[i];
  }
  
  PEstados = new TGraph(N,s,Res);
  PEstados->SetFillColor(40);
  PEstados->SetTitle("States Amplitude; Elements; Amplitude");
  
  Steps = new TPaveText(0.2,0.1,0.8,0.9);
  Steps->SetTextAlign(12);
  Steps->SetShadowColor(0);
  Steps->SetFillColor(0);
  Steps->SetLineColor(0);
  Steps->SetTextSize(0.05);
  Steps->AddText("Initialize the n qubits to state |0#GT"); 
  Steps->AddText("Apply Hadamard Gate over the n qubits:");
  Steps->AddText("H^{#otimesn}|0#GT^{#otimesn}");
  Steps->AddText("Apply a sign shift with the oracle U_{f} ");((TText*)Steps->GetListOfLines()->Last())->SetTextColor(kOrange+1);
  Steps->AddText("Apply the Diffusion transform D");
  Steps->AddText("Measure the states (qubits)");

  StepGates = new TPaveText(0.2,0.1,0.8,0.9);
  StepGates->SetTextAlign(12);
  StepGates->SetShadowColor(0);
  StepGates->SetFillColor(0);
  StepGates->SetLineColor(0);
  StepGates->SetTextSize(0.05);
  StepGates->AddText("Apply oracle U_{f}:");
  StepGates->AddText("#color[22]{   U_{f} = #lower[0.2]{#scale[4.5]{(}} #splitline{#splitline{1-2f(0)}{   0}}{#splitline{#splitline{  #upoint}{#lower[-0.5]{  #upoint}}}{#splitline{#lower[-0.5]{  #upoint}}{#lower[-0.3]{   0}}}}    #splitline{#splitline{   0}{1-2f(1)}}{#splitline{#splitline{  #upoint}{#lower[-0.5]{  #upoint}}}{#splitline{#lower[-0.5]{  #upoint}}{#lower[-0.3]{   0}}}}   #3dots   #splitline{#splitline{   0}{   0}}{#splitline{#splitline{  #upoint}{#lower[-0.5]{  #upoint}}}{#splitline{#lower[-0.5]{  #upoint}}{#lower[-0.3]{1-2f(N)}}}} #lower[0.2]{#scale[4.5]{)}}}");
  StepGates->AddText("");
  StepGates->AddText("The oracle U_{f} produces a sign shift over the search element");
  

  double xf=sqrt((N-1)*(1./N));
  double yf=-sqrt(1./N);
  SVector = new TArrow(0.0,0.0,xf,yf,0.015,"|>");
  SVector->SetFillColor(2);
  SVector->SetLineColor(2);
  
  switch(n){
  case 2:
    fCanvas->cd(1);

    C->Draw();

    Circuit1 = new TLine(0.06,0.7,0.99,0.7);
    Circuit2 = new TLine(0.06,0.3,0.99,0.3);
    
    HGate = new TPaveText(0.08,0.624,0.16,0.774);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");
    
    HGate2 = new TPaveText(0.08,0.224,0.16,0.374);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");
    
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.685,"|0#GT");
    latex->DrawLatex(0.02,0.285,"|0#GT");
    
    UfGate = new TPaveText(0.20,0.224,0.28,0.774);
    UfGate->SetTextAlign(22);
    UfGate->SetShadowColor(0);
    UfGate->SetTextFont(11);
    UfGate->SetTextSize(0.07);
    UfGate->SetFillColor(22);
    UfGate->AddText("Uf");

    Circuit1->Draw();
    Circuit2->Draw();
    HGate->Draw();
    HGate2->Draw();
    UfGate->Draw();
    // UfGate2->Draw();

    gCanvas->cd(1);

    latex->DrawLatex(0.02,0.685,"|0#GT");
    latex->DrawLatex(0.02,0.285,"|0#GT");
    Circuit1->Draw();
    Circuit2->Draw();
    HGate->Draw();
    HGate2->Draw();
    UfGate->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();

    gCanvas->cd(2);
    
    StepGates->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.03,yf-0.12,"U_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.03,yf-0.12,"U_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();
    
    fCanvas->cd(4);
    
    PEstados->Draw("AB");

    gCanvas->cd(4);
    
    PEstados->Draw("AB");
    
    fCanvas->Update();
    gCanvas->Update();
    
    break;
  case 3:
    
    fCanvas->cd(1);

    C->Draw();
    
    Circuit1 = new TLine(0.06,0.75,0.99,0.75);
    Circuit2 = new TLine(0.06,0.5,0.99,0.5);
    Circuit3 = new TLine(0.06,0.25,0.99,0.25);
    
    HGate = new TPaveText(0.08,0.674,0.16,0.824);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");
    
    HGate2 = new TPaveText(0.08,0.424,0.16,0.574);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");

    HGate3 = new TPaveText(0.08,0.174,0.16,0.324);
    HGate3->SetTextAlign(22);
    HGate3->SetShadowColor(0);
    HGate3->SetTextFont(11);
    HGate3->SetTextSize(0.07);
    HGate3->SetFillColor(20);
    HGate3->AddText("H");

    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.485,"|0#GT");
    latex->DrawLatex(0.02,0.235,"|0#GT");

    UfGate = new TPaveText(0.20,0.174,0.28,0.824);
    UfGate->SetTextAlign(22);
    UfGate->SetShadowColor(0);
    UfGate->SetTextFont(11);
    UfGate->SetTextSize(0.07);
    UfGate->SetFillColor(22);
    UfGate->AddText("Uf");

    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();

    HGate->Draw();
    HGate2->Draw();
    HGate3->Draw();
    
    UfGate->Draw();
    //UfGate2->Draw();
    //UfGate3->Draw();

    gCanvas->cd(1);

    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.485,"|0#GT");
    latex->DrawLatex(0.02,0.235,"|0#GT");
    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();

    HGate->Draw();
    HGate2->Draw();
    HGate3->Draw();
    
    UfGate->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();

    gCanvas->cd(2);
    
    StepGates->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.1,yf-0.14,"U_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.1,yf-0.14,"U_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();
    
    fCanvas->cd(4);
    
    PEstados->Draw("AB");

    gCanvas->cd(4);
    
    PEstados->Draw("AB");
    
    fCanvas->Update();
    gCanvas->Update();
    
    break;
  case 4:
    fCanvas->cd(1);
    
    C->Draw();
    
    Circuit1 = new TLine(0.06,0.75,0.99,0.75);
    Circuit2 = new TLine(0.06,0.55,0.99,0.55);
    Circuit3 = new TLine(0.06,0.35,0.99,0.35);
    Circuit4 = new TLine(0.06,0.15,0.99,0.15);
    
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.535,"|0#GT");
    latex->DrawLatex(0.02,0.335,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");
    
    HGate = new TPaveText(0.08,0.674,0.16,0.824);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");
    
    HGate2 = new TPaveText(0.08,0.474,0.16,0.624);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");

    HGate3 = new TPaveText(0.08,0.274,0.16,0.424);
    HGate3->SetTextAlign(22);
    HGate3->SetShadowColor(0);
    HGate3->SetTextFont(11);
    HGate3->SetTextSize(0.07);
    HGate3->SetFillColor(20);
    HGate3->AddText("H");

    HGate4 = new TPaveText(0.08,0.074,0.16,0.224);
    HGate4->SetTextAlign(22);
    HGate4->SetShadowColor(0);
    HGate4->SetTextFont(11);
    HGate4->SetTextSize(0.07);
    HGate4->SetFillColor(20);
    HGate4->AddText("H");

    UfGate = new TPaveText(0.20,0.074,0.28,0.824);
    UfGate->SetTextAlign(22);
    UfGate->SetShadowColor(0);
    UfGate->SetTextFont(11);
    UfGate->SetTextSize(0.07);
    UfGate->SetFillColor(22);
    UfGate->AddText("Uf");

    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();
    Circuit4->Draw();

    HGate->Draw();
    HGate2->Draw();
    HGate3->Draw();
    HGate4->Draw();

    UfGate->Draw();
    //UfGate2->Draw();
    //UfGate3->Draw();
    //UfGate4->Draw();

    gCanvas->cd(1);

    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.535,"|0#GT");
    latex->DrawLatex(0.02,0.335,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");
    
    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();
    Circuit4->Draw();

    HGate->Draw();
    HGate2->Draw();
    HGate3->Draw();
    HGate4->Draw();

    UfGate->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();

    gCanvas->cd(2);
    
    StepGates->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.1,yf-0.14,"U_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.1,yf-0.14,"U_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");

    gCanvas->cd(4);
    
    PEstados->Draw("AB");

    fCanvas->Update();
    gCanvas->Update();
    
    break;
  }

  if(n>4){

    fCanvas->cd(1)->SetPad(0.01,0.51,0.85,0.99);
    
    C->Draw();
    
    Circuit1 = new TLine(0.06,0.75,0.99,0.75);
    Circuit2 = new TLine(0.06,0.15,0.99,0.15);
    
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");
    latex->SetTextSize(0.2);
    latex->DrawLatex(0.03,0.56,"#upoint");
    latex->DrawLatex(0.03,0.41,"#upoint");
    latex->DrawLatex(0.03,0.26,"#upoint");
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.0,0.45,"n");
    
    HGate = new TPaveText(0.08,0.674,0.125,0.824);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");
    
    HGate2 = new TPaveText(0.08,0.074,0.125,0.224);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");

    UfGate = new TPaveText(0.145,0.074,0.19,0.824);
    UfGate->SetTextAlign(22);
    UfGate->SetShadowColor(0);
    UfGate->SetTextFont(11);
    UfGate->SetTextSize(0.07);
    UfGate->SetFillColor(22);
    UfGate->AddText("Uf");

    Circuit1->Draw();
    Circuit2->Draw();

    HGate->Draw();
    HGate2->Draw();

    UfGate->Draw();

    gCanvas->cd(1)->SetPad(0.01,0.51,0.85,0.99);

    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");
    latex->SetTextSize(0.2);
    latex->DrawLatex(0.03,0.56,"#upoint");
    latex->DrawLatex(0.03,0.41,"#upoint");
    latex->DrawLatex(0.03,0.26,"#upoint");
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.0,0.45,"n");

    C->Draw();
    Circuit1->Draw();
    Circuit2->Draw();

    HGate->Draw();
    HGate2->Draw();

    UfGate->Draw();
    
    fCanvas->cd(2)->SetPad(0.86,0.51,0.99,0.99);
    gCanvas->cd(2)->SetPad(0.86,0.51,0.99,0.99);
    
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.1,yf-0.14,"U_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf-0.1,yf-0.14,"U_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");

    gCanvas->cd(4);
    
    PEstados->Draw("AB");

    fCanvas->Update();
    gCanvas->Update();

  }

  previous->Disconnect("Clicked()");
  next->Disconnect("Clicked()");
  previous->Connect("Clicked()","MyMainFrame",this,"Step1()");
  next->Connect("Clicked()","MyMainFrame",this,"Diffuser()");
}
// End Orcale

//..............................................................................

//Begin Diffuser

void MyMainFrame::Diffuser(){

  fCanvas->Clear();
  fCanvas->Update();
  fCanvas->Divide(2,2);
  gCanvas->Clear();
  gCanvas->Update();
  gCanvas->Divide(2,2);
  GateColor=gROOT->GetColor(20);
  GateColor->SetRGB(0.22,0.33,0.61);
  DColor=gROOT->GetColor(21);
  DColor->SetRGB(0.34,0.60,0.73);
  UFColor=gROOT->GetColor(22);
  UFColor->SetRGB(0.58,0.47,0.90);
  
  TText *C = new TText(0.5,0.95,"Circuit");
  C->SetTextAlign(22);
  C->SetTextColor(kBlue+2);
  C->SetTextFont(22);
  C->SetTextSize(0.09);

  TText *GV = new TText(-0.57,0.9, "Geometric Visualization");
  GV->SetTextAlign(22);
  GV->SetTextColor(kBlue+2);
  GV->SetTextFont(22);
  GV->SetTextSize(0.08);

  latex = new TLatex();
  
  double dif[N*N];
  
  for(int i=0; i<=(N*N); i++){
    dif[i]=(2./N);
  }
  
  for(int i=1; i<=N; i++){
    
    int x = (i-1)+(N*(i-1)); 
    
    dif[x]=-1+(2./N);
    
  }
  
  TMatrixT<double> Difusser(N,N,dif);
  
  double s[N];
  
  for(int i=0; i<=N; i++){
    
    s[i]=i+1;
  }

  for(int i=0; i<=N; i++){
    
    ms[i]=M_state[i];
 
  }

  TMatrixT<double> State(N,1,ms);

  TMatrixT<double> DResult = Difusser*State;

  double *Res = DResult.GetMatrixArray();

   for(int i=0; i<=N; i++){
    
    ms[i]=Res[i];
  }

  PEstados = new TGraph(N,s,Res);
  PEstados->SetFillColor(40);
  PEstados->SetTitle("States Amplitude; Elements; Amplitude");
  
  Steps = new TPaveText(0.2,0.1,0.8,0.9);
  Steps->SetTextAlign(12);
  Steps->SetShadowColor(0);
  Steps->SetFillColor(0);
  Steps->SetLineColor(0);
  Steps->SetTextSize(0.05);
  Steps->AddText("Initialize the n qubits to state |0#GT"); 
  Steps->AddText("Apply Hadamard Gate over the n qubits:");
  Steps->AddText("H^{#otimesn}|0#GT^{#otimesn}");
  Steps->AddText("Apply a sign shift with the oracle U_{f} ");
  Steps->AddText("Apply the Diffusion transform D");((TText*)Steps->GetListOfLines()->Last())->SetTextColor(kOrange+1);
  Steps->AddText("Measure the states (qubits)");

  StepGates = new TPaveText(0.2,0.1,0.8,0.9);
  StepGates->SetTextAlign(12);
  StepGates->SetShadowColor(0);
  StepGates->SetFillColor(0);
  StepGates->SetLineColor(0);
  StepGates->SetTextSize(0.05);
  StepGates->AddText("Apply the diffusion transform D:");
  StepGates->AddText("");
  StepGates->AddText("#color[21]{   D = #lower[0.2]{#scale[6]{(}} #splitline{#splitline{-1+#frac{2}{N}}{  #frac{2}{N}}}{#splitline{#splitline{   #upoint}{#lower[-0.5]{   #upoint}}}{#splitline{#lower[-0.5]{   #upoint}}{  #frac{2}{N}}}}    #splitline{#splitline{  #frac{2}{N}}{-1+#frac{2}{N}}}{#splitline{#splitline{   #upoint}{#lower[-0.5]{   #upoint}}}{#splitline{#lower[-0.5]{   #upoint}}{  #frac{2}{N}}}}   #3dots   #splitline{#splitline{  #frac{2}{N}}{  #frac{2}{N}}}{#splitline{#splitline{   #upoint}{#lower[-0.5]{   #upoint}}}{#splitline{#lower[-0.5]{   #upoint}}{-1+#frac{2}{N}}}} #lower[0.2]{#scale[6]{)}}}");
  StepGates->AddText("");
  StepGates->AddText("The diffusion transform perofrms an inversion over the average");
  StepGates->AddText("incrementing the amplitude of the search element (state).");

  double xf;
  double yf;
  if(k<N){
    xf=sqrt(ms[k]*ms[k]*(N-1));
    yf=ms[k-1];
  }
  else{
    xf=sqrt(ms[0]*ms[0]*(N-1));
    yf=ms[k-1];
  }
  SVector = new TArrow(0.0,0.0,xf,yf,0.015,"|>");
  SVector->SetFillColor(2);
  SVector->SetLineColor(2);
  
  switch(n){
  case 2:
    fCanvas->cd(1);

    C->Draw();

    Circuit1 = new TLine(0.06,0.7,0.99,0.7);
    Circuit2 = new TLine(0.06,0.3,0.99,0.3);
    
    HGate = new TPaveText(0.08,0.624,0.16,0.774);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");
    
    HGate2 = new TPaveText(0.08,0.224,0.16,0.374);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");
    
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.685,"|0#GT");
    latex->DrawLatex(0.02,0.285,"|0#GT");
    
    UfGate = new TPaveText(0.20,0.224,0.28,0.774);
    UfGate->SetTextAlign(22);
    UfGate->SetShadowColor(0);
    UfGate->SetTextFont(11);
    UfGate->SetTextSize(0.07);
    UfGate->SetFillColor(22);
    UfGate->AddText("Uf");

    DifGate = new TPaveText(0.32,0.224,0.40,0.774);
    DifGate->SetTextAlign(22);
    DifGate->SetShadowColor(0);
    DifGate->SetTextFont(11);
    DifGate->SetTextSize(0.07);
    DifGate->SetFillColor(21);
    DifGate->AddText("D");

    Circuit1->Draw();
    Circuit2->Draw();
    HGate->Draw();
    HGate2->Draw();
    UfGate->Draw();
    //UfGate2->Draw();
    
    DifGate->Draw();
    // DifGate2->Draw();

    gCanvas->cd(1);

    latex->DrawLatex(0.02,0.685,"|0#GT");
    latex->DrawLatex(0.02,0.285,"|0#GT");

    Circuit1->Draw();
    Circuit2->Draw();
    HGate->Draw();
    HGate2->Draw();
    UfGate->Draw();
    DifGate->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();

    gCanvas->cd(2);
    
    StepGates->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf+0.02,yf-0.08,"DU_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    SVector->Draw();
    GV->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf+0.02,yf-0.08,"DU_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    SVector->Draw();
    GV->Draw();
    
    fCanvas->cd(4);
    
    PEstados->Draw("AB");

    gCanvas->cd(4);
    
    PEstados->Draw("AB");
    
    fCanvas->Update();
    gCanvas->Update();
    
    break;
  case 3:
    fCanvas->cd(1);

    C->Draw();
    
    Circuit1 = new TLine(0.06,0.75,0.99,0.75);
    Circuit2 = new TLine(0.06,0.5,0.99,0.5);
    Circuit3 = new TLine(0.06,0.25,0.99,0.25);
    
    HGate = new TPaveText(0.08,0.674,0.16,0.824);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");
    
    HGate2 = new TPaveText(0.08,0.424,0.16,0.574);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");

    HGate3 = new TPaveText(0.08,0.174,0.16,0.324);
    HGate3->SetTextAlign(22);
    HGate3->SetShadowColor(0);
    HGate3->SetTextFont(11);
    HGate3->SetTextSize(0.07);
    HGate3->SetFillColor(20);
    HGate3->AddText("H");

    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.485,"|0#GT");
    latex->DrawLatex(0.02,0.235,"|0#GT");

    UfGate = new TPaveText(0.20,0.174,0.28,0.824);
    UfGate->SetTextAlign(22);
    UfGate->SetShadowColor(0);
    UfGate->SetTextFont(11);
    UfGate->SetTextSize(0.07);
    UfGate->SetFillColor(22);
    UfGate->AddText("Uf");
    
    DifGate = new TPaveText(0.32,0.174,0.40,0.824);
    DifGate->SetTextAlign(22);
    DifGate->SetShadowColor(0);
    DifGate->SetTextFont(11);
    DifGate->SetTextSize(0.07);
    DifGate->SetFillColor(21);
    DifGate->AddText("D");

    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();

    HGate->Draw();
    HGate2->Draw();
    HGate3->Draw();
    
    UfGate->Draw();
    // UfGate2->Draw();
    // UfGate3->Draw();
    
    DifGate->Draw();
    // DifGate2->Draw();
    // DifGate3->Draw();

    gCanvas->cd(1);

    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.485,"|0#GT");
    latex->DrawLatex(0.02,0.235,"|0#GT");
    
    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();

    HGate->Draw();
    HGate2->Draw();
    HGate3->Draw();
    
    UfGate->Draw();
 
    DifGate->Draw();
    
    
    fCanvas->cd(2);
    
    Steps->Draw();

    gCanvas->cd(2);
    
    StepGates->Draw();
    
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf+0.02,yf-0.08,"DU_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    SVector->Draw();
    GV->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf+0.02,yf-0.08,"DU_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    SVector->Draw();
    GV->Draw();
    
    fCanvas->cd(4);
    
    PEstados->Draw("AB");

    gCanvas->cd(4);
    
    PEstados->Draw("AB");
    
    fCanvas->Update();
    gCanvas->Update();
    
    break;
  case 4:
    fCanvas->cd(1);
    
    C->Draw();
    
    Circuit1 = new TLine(0.06,0.75,0.99,0.75);
    Circuit2 = new TLine(0.06,0.55,0.99,0.55);
    Circuit3 = new TLine(0.06,0.35,0.99,0.35);
    Circuit4 = new TLine(0.06,0.15,0.99,0.15);
    
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.535,"|0#GT");
    latex->DrawLatex(0.02,0.335,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");
    
    HGate = new TPaveText(0.08,0.674,0.16,0.824);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");
    
    HGate2 = new TPaveText(0.08,0.474,0.16,0.624);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");

    HGate3 = new TPaveText(0.08,0.274,0.16,0.424);
    HGate3->SetTextAlign(22);
    HGate3->SetShadowColor(0);
    HGate3->SetTextFont(11);
    HGate3->SetTextSize(0.07);
    HGate3->SetFillColor(20);
    HGate3->AddText("H");

    HGate4 = new TPaveText(0.08,0.074,0.16,0.224);
    HGate4->SetTextAlign(22);
    HGate4->SetShadowColor(0);
    HGate4->SetTextFont(11);
    HGate4->SetTextSize(0.07);
    HGate4->SetFillColor(20);
    HGate4->AddText("H");

    UfGate = new TPaveText(0.20,0.074,0.28,0.824);
    UfGate->SetTextAlign(22);
    UfGate->SetShadowColor(0);
    UfGate->SetTextFont(11);
    UfGate->SetTextSize(0.07);
    UfGate->SetFillColor(22);
    UfGate->AddText("Uf");

    
    DifGate = new TPaveText(0.32,0.074,0.40,0.824);
    DifGate->SetTextAlign(22);
    DifGate->SetShadowColor(0);
    DifGate->SetTextFont(11);
    DifGate->SetTextSize(0.07);
    DifGate->SetFillColor(21);
    DifGate->AddText("D");

    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();
    Circuit4->Draw();

    HGate->Draw();
    HGate2->Draw();
    HGate3->Draw();
    HGate4->Draw();

    UfGate->Draw();
    // UfGate2->Draw();
    // UfGate3->Draw();
    //UfGate4->Draw();

    DifGate->Draw();
    // DifGate2->Draw();
    //DifGate3->Draw();
    //DifGate4->Draw();

    gCanvas->cd(1);

    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.535,"|0#GT");
    latex->DrawLatex(0.02,0.335,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");
    
    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();
    Circuit4->Draw();

    HGate->Draw();
    HGate2->Draw();
    HGate3->Draw();
    HGate4->Draw();

    UfGate->Draw();

    DifGate->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();

    gCanvas->cd(2);
    
    StepGates->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf+0.01,yf-0.14,"DU_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf+0.01,yf-0.14,"DU_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");

    gCanvas->cd(4);
    
    PEstados->Draw("AB");

    fCanvas->Update();
    gCanvas->Update();
     
    break;

  }

  if(n>4){

    fCanvas->cd(1)->SetPad(0.01,0.51,0.85,0.99);
    
    C->Draw();
    
    Circuit1 = new TLine(0.06,0.75,0.99,0.75);
    Circuit2 = new TLine(0.06,0.15,0.99,0.15);
    
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");
    latex->SetTextSize(0.2);
    latex->DrawLatex(0.03,0.56,"#upoint");
    latex->DrawLatex(0.03,0.41,"#upoint");
    latex->DrawLatex(0.03,0.26,"#upoint");
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.0,0.45,"n");
    
    HGate = new TPaveText(0.08,0.674,0.125,0.824);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");

    HGate2 = new TPaveText(0.08,0.074,0.125,0.224);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");

    UfGate = new TPaveText(0.145,0.074,0.19,0.824);
    UfGate->SetTextAlign(22);
    UfGate->SetShadowColor(0);
    UfGate->SetTextFont(11);
    UfGate->SetTextSize(0.07);
    UfGate->SetFillColor(22);
    UfGate->AddText("Uf");
    
    
    DifGate = new TPaveText(0.21,0.074,0.255,0.824);
    DifGate->SetTextAlign(22);
    DifGate->SetShadowColor(0);
    DifGate->SetTextFont(11);
    DifGate->SetTextSize(0.07);
    DifGate->SetFillColor(21);
    DifGate->AddText("D");

    Circuit1->Draw();
    Circuit2->Draw();

    HGate->Draw();
    HGate2->Draw();

    UfGate->Draw();

    DifGate->Draw();

    gCanvas->cd(1)->SetPad(0.01,0.51,0.85,0.99);

    latex->DrawLatex(0.02,0.735,"|0#GT");
    latex->DrawLatex(0.02,0.135,"|0#GT");
    latex->SetTextSize(0.2);
    latex->DrawLatex(0.03,0.56,"#upoint");
    latex->DrawLatex(0.03,0.41,"#upoint");
    latex->DrawLatex(0.03,0.26,"#upoint");
    latex->SetTextSize(0.05);
    latex->DrawLatex(0.0,0.45,"n");

    C->Draw();
    Circuit1->Draw();
    Circuit2->Draw();

    HGate->Draw();
    HGate2->Draw();

    UfGate->Draw();

    DifGate->Draw();
    
    fCanvas->cd(2)->SetPad(0.86,0.51,0.99,0.99);
    gCanvas->cd(2)->SetPad(0.86,0.51,0.99,0.99);
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    
    latex->SetTextSize(0.06);
    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf+0.01,yf-0.14,"DU_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();

    gCanvas->cd(3)->Range(-1,-1,1,1);

    latex->DrawLatex(-0.07,0.89,"|#beta#GT");
    latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex->DrawLatex(xf+0.01,yf-0.14,"DU_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    GV->Draw();
    SVector->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");

    gCanvas->cd(4);
    
    PEstados->Draw("AB");

    fCanvas->Update();
    gCanvas->Update();

  }

  previous->Disconnect("Clicked()");
  next->Disconnect("Clicked()");
  previous->Connect("Clicked()","MyMainFrame",this,"Oracle()");
  next->Connect("Clicked()","MyMainFrame",this,"Repeat()");

  if(R==1){
     next->Disconnect("Clicked()");
     next->Connect("Clicked()","MyMainFrame",this,"Measure()");
  }

}//End Diffuser

//----------------------------------------------------------------------------

//Begin Repeat

void MyMainFrame::Repeat(){

  Chrono = new TTimer(1500);
  Chrono->Connect("Timeout()","MyMainFrame",this,"AnimR()");
  r=1;
  half=0;
  previous->SetState(kButtonDisabled, kTRUE);
  next->SetState(kButtonDisabled, kTRUE);
  Chrono->TurnOn();

  previous->Disconnect("Clicked()");
  next->Disconnect("Clicked()");
  previous->Connect("Clicked()","MyMainFrame",this,"Diffuser()");
  next->Connect("Clicked()","MyMainFrame",this,"Measure()");

}

void MyMainFrame::AnimR(){

  fCanvas->Update();
  DColor->SetRGB(0.34,0.60,0.73);
  UFColor->SetRGB(0.58,0.47,0.90);

  latex = new TLatex();
  

  TText *GV = new TText(-0.57,0.9, "Geometric Visualization");
  GV->SetTextAlign(22);
  GV->SetTextColor(kBlue+2);
  GV->SetTextFont(22);
  GV->SetTextSize(0.08);

  if(half==1){
  
    double dif[N*N];
    
    for(int i=0; i<=(N*N); i++){
      dif[i]=(2./N);
    }
    
    for(int i=1; i<=N; i++){
      
      int x = (i-1)+(N*(i-1)); 
      
      dif[x]=-1+(2./N);
      
    }
    
    TMatrixT<double> Difusser(N,N,dif);
    
    double s[N];
    
    for(int i=0; i<=N; i++){
      
      s[i]=i+1;
    }
    
    TMatrixT<double> State(N,1,ms);
    
    TMatrixT<double> DResult = Difusser*State;
    
    double *Res = DResult.GetMatrixArray();
    
    for(int i=0; i<=N; i++){
      
      ms[i]=Res[i];
    }
    
    PEstados = new TGraph(N,s,Res);
    PEstados->SetFillColor(40);
    PEstados->SetTitle("States Amplitude; Elements; Amplitude");
    
    Steps = new TPaveText(0.2,0.1,0.8,0.9);
    Steps->SetTextAlign(12);
    Steps->SetShadowColor(0);
    Steps->SetFillColor(0);
    Steps->SetLineColor(0);
    Steps->SetTextSize(0.05);
    Steps->AddText("Initialize the n qubits to state |0#GT"); 
    Steps->AddText("Apply Hadamard Gate over the n qubits:");
    Steps->AddText("H^{#otimesn}|0#GT^{#otimesn}");
    Steps->AddText("Apply a sign shift with the oracle U_{f} ");
    Steps->AddText("Apply the Diffusion transform D");((TText*)Steps->GetListOfLines()->Last())->SetTextColor(kOrange+1);
    Steps->AddText("Measure the states (qubits)");
    
    StepGates = new TPaveText(0.2,0.1,0.8,0.9);
    StepGates->SetTextAlign(12);
    StepGates->SetShadowColor(0);
    StepGates->SetFillColor(0);
    StepGates->SetLineColor(0);
    StepGates->SetTextSize(0.05);
    StepGates->AddText("Apply the diffusion transform D:");
    StepGates->AddText("");
    StepGates->AddText("#color[21]{   D = #lower[0.2]{#scale[6]{(}} #splitline{#splitline{-1+#frac{2}{N}}{  #frac{2}{N}}}{#splitline{#splitline{   #upoint}{#lower[-0.5]{   #upoint}}}{#splitline{#lower[-0.5]{   #upoint}}{  #frac{2}{N}}}}    #splitline{#splitline{  #frac{2}{N}}{-1+#frac{2}{N}}}{#splitline{#splitline{   #upoint}{#lower[-0.5]{   #upoint}}}{#splitline{#lower[-0.5]{   #upoint}}{  #frac{2}{N}}}}   #3dots   #splitline{#splitline{  #frac{2}{N}}{  #frac{2}{N}}}{#splitline{#splitline{   #upoint}{#lower[-0.5]{   #upoint}}}{#splitline{#lower[-0.5]{   #upoint}}{-1+#frac{2}{N}}}} #lower[0.2]{#scale[6]{)}}}");
    StepGates->AddText("");
    StepGates->AddText("The diffusion transform perofrms an inversion over the average");
    StepGates->AddText("incrementing the amplitude of the search element (state).");
    
    
    double xf;
    double yf;
    if(k<N){
      xf=sqrt(ms[k]*ms[k]*(N-1));
      yf=ms[k-1];
    }
    else{
      xf=sqrt(ms[0]*ms[0]*(N-1));
      yf=ms[k-1];
    }
    SVector = new TArrow(0.0,0.0,xf,yf,0.015,"|>");
    SVector->SetFillColor(2);
    SVector->SetLineColor(2);

    double xinextg=0.4+0.08*r+0.08*(r+1*(r-1));
    double xfnextg=0.48+0.08*r+0.08*(r+1*(r-1));
    
    switch(n){
    case 2:
      fCanvas->cd(1);
    
      DifGate = new TPaveText(xinextg,0.224,xfnextg,0.774);
      DifGate->SetTextAlign(22);
      DifGate->SetShadowColor(0);
      DifGate->SetTextFont(11);
      DifGate->SetTextSize(0.07);
      DifGate->SetFillColor(21);
      DifGate->AddText("D");
      
      DifGate->Draw();
      //DifGate2->Draw();

      gCanvas->cd(1);

      DifGate->Draw();
      
      fCanvas->cd(2);
    
      Steps->Draw();

      gCanvas->cd(2);
    
      StepGates->Draw();

      
      fCanvas->cd(3)->Range(-1,-1,1,1);
      fCanvas->cd(3)->Clear();
      
      Axis1 = new TLine(0.0,0.0,0.9,0.0);
      Axis2 = new TLine(0.0,-0.9,0.0,0.85);
      //Axis3 = new TLine(0,0,-0.316,-0.589);
      
      latex->SetTextSize(0.06);
      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
      //latex->DrawLatex(xf+0.02,yf-0.08,"DU_{f}|#varphi#GT");
      
      Axis1->Draw();
      Axis2->Draw();
      SVector->Draw();
      GV->Draw();

      gCanvas->cd(3)->Range(-1,-1,1,1);
      gCanvas->cd(3)->Clear();

      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
      
      Axis1->Draw();
      Axis2->Draw();
      SVector->Draw();
      GV->Draw();
      
      fCanvas->cd(4);
      
      PEstados->Draw("AB");

      gCanvas->cd(4);
      
      PEstados->Draw("AB");
      
      fCanvas->Update();
      gCanvas->Update();
      
      break;
    case 3:
      fCanvas->cd(1);
      
      DifGate = new TPaveText(xinextg,0.174,xfnextg,0.824);
      DifGate->SetTextAlign(22);
      DifGate->SetShadowColor(0);
      DifGate->SetTextFont(11);
      DifGate->SetTextSize(0.07);
      DifGate->SetFillColor(21);
      DifGate->AddText("D");
      
      DifGate->Draw();
      //DifGate2->Draw();
      //DifGate3->Draw();

      gCanvas->cd(1);
      DifGate->Draw();
      
      fCanvas->cd(2);
      fCanvas->cd(2)->Clear();
      Steps->Draw();

      gCanvas->cd(2);
      gCanvas->cd(2)->Clear();
      StepGates->Draw();
      
      fCanvas->cd(3)->Range(-1,-1,1,1);
      fCanvas->cd(3)->Clear();
      
      Axis1 = new TLine(0.0,0.0,0.9,0.0);
      Axis2 = new TLine(0.0,-0.9,0.0,0.85);
      //Axis3 = new TLine(0,0,-0.316,-0.589);
      
      latex->SetTextSize(0.06);
      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
      //latex->DrawLatex(xf+0.02,yf-0.08,"DU_{f}|#varphi#GT");
      
      Axis1->Draw();
      Axis2->Draw();
      SVector->Draw();
      GV->Draw();

      gCanvas->cd(3)->Range(-1,-1,1,1);
      gCanvas->cd(3)->Clear();

      latex->SetTextSize(0.06);
      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
      //latex->DrawLatex(xf+0.02,yf-0.08,"DU_{f}|#varphi#GT");
      
      Axis1->Draw();
      Axis2->Draw();
      SVector->Draw();
      GV->Draw();
      
      fCanvas->cd(4);
      
      PEstados->Draw("AB");

      gCanvas->cd(4);
      
      PEstados->Draw("AB");
      
      fCanvas->Update();
      gCanvas->Update();
      
      break;
    case 4:
      fCanvas->cd(1);
      
      DifGate = new TPaveText(xinextg,0.074,xfnextg,0.824);
      DifGate->SetTextAlign(22);
      DifGate->SetShadowColor(0);
      DifGate->SetTextFont(11);
      DifGate->SetTextSize(0.07);
      DifGate->SetFillColor(21);
      DifGate->AddText("D");
            
      DifGate->Draw();
      //DifGate2->Draw();
      //DifGate3->Draw();
      //DifGate4->Draw();

      gCanvas->cd(1);
      DifGate->Draw();
      
      fCanvas->cd(2);
      fCanvas->cd(2)->Clear();
      Steps->Draw();

      gCanvas->cd(2);
      gCanvas->cd(2)->Clear();
      StepGates->Draw();
      
      fCanvas->cd(3)->Range(-1,-1,1,1);
      fCanvas->cd(3)->Clear();
      
      Axis1 = new TLine(0.0,0.0,0.9,0.0);
      Axis2 = new TLine(0.0,-0.9,0.0,0.85);
      //Axis3 = new TLine(0,0,-0.316,-0.589);
      
      latex->SetTextSize(0.06);
      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
      //latex->DrawLatex(xf,yf,"DU_{f}|#varphi#GT");
      
      Axis1->Draw();
      Axis2->Draw();
      GV->Draw();
      SVector->Draw();

      gCanvas->cd(3)->Range(-1,-1,1,1);
      gCanvas->cd(3)->Clear();

      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
      
      Axis1->Draw();
      Axis2->Draw();
      GV->Draw();
      SVector->Draw();
      
      fCanvas->cd(4);
      
      fCanvas->SetTitle("Circuit");
      PEstados->Draw("AB");

      gCanvas->cd(4);
      
      PEstados->Draw("AB");
      
      fCanvas->Update();
      gCanvas->Update();
      
      break;
    }

    if(n>4){

      double xinextg=0.255+0.045*(2*r-1)+0.04*r;
      double xfnextg=0.3+0.045*(2*r-1)+0.04*r;

      fCanvas->cd(1)->SetPad(0.01,0.51,0.85,0.99);
      
      DifGate = new TPaveText(xinextg,0.074,xfnextg,0.824);
      DifGate->SetTextAlign(22);
      DifGate->SetShadowColor(0);
      DifGate->SetTextFont(11);
      DifGate->SetTextSize(0.07);
      DifGate->SetFillColor(21);
      DifGate->AddText("D");
      
      
      DifGate->Draw();

      gCanvas->cd(1)->SetPad(0.01,0.51,0.85,0.99);

      DifGate->Draw();
      
      fCanvas->cd(2)->SetPad(0.86,0.51,0.99,0.99);
      fCanvas->cd(2)->Clear();

      gCanvas->cd(2)->SetPad(0.86,0.51,0.99,0.99);
      gCanvas->cd(2)->Clear();
      
      fCanvas->cd(3)->Range(-1,-1,1,1);
      fCanvas->cd(3)->Clear();
      
      Axis1 = new TLine(0.0,0.0,0.9,0.0);
      Axis2 = new TLine(0.0,-0.9,0.0,0.85);
      
      latex->SetTextSize(0.06);
      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
      //latex->DrawLatex(xf,yf,"DU_{f}|#varphi#GT");
      
      Axis1->Draw();
      Axis2->Draw();
      GV->Draw();
      SVector->Draw();

      gCanvas->cd(3)->Range(-1,-1,1,1);
      gCanvas->cd(3)->Clear();

      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
      
      Axis1->Draw();
      Axis2->Draw();
      GV->Draw();
      SVector->Draw();
      
      fCanvas->cd(4);
      
      fCanvas->SetTitle("Circuit");
      PEstados->Draw("AB");

      gCanvas->cd(4);
      
      PEstados->Draw("AB");
      
      fCanvas->Update();
      gCanvas->Update();
      
    }
    
    half=0;
    r=r+1;
  }
  else{
    
    double uf[N*N];
    
    for(int i=0; i<=(N*N); i++){
      uf[i]=0.0;
      
      //std::cout<<uf[0]<<std::endl;
    }
    
    for(int i=1; i<=N; i++){
      
      int x = (i-1)+(N*(i-1)); 
      
      uf[x]=1;
      
      }
    
    int s_e = (k-1)+(N*(k-1));
    
    uf[s_e]=-1;
    
    TMatrixT<double> Uf(N,N,uf);
    
    double s[N];
    
    for(int i=0; i<=N; i++){
      
	s[i]=i+1;
    }
    
    TMatrixT<double> State(N,1,ms);
    
    TMatrixT<double> OResult = Uf*State;
    
    double *Res = OResult.GetMatrixArray();
    
    for(int i=0; i<=N; i++){
      
      ms[i]=Res[i];
    }
    
    PEstados = new TGraph(N,s,Res);
    PEstados->SetFillColor(40);
    PEstados->SetTitle("States Amplitude; Elements; Amplitude");
    
    Steps = new TPaveText(0.2,0.1,0.8,0.9);
    Steps->SetTextAlign(12);
    Steps->SetShadowColor(0);
    Steps->SetFillColor(0);
    Steps->SetLineColor(0);
    Steps->SetTextSize(0.05);
    Steps->AddText("Initialize the n qubits to state |0#GT"); 
    Steps->AddText("Apply Hadamard Gate over the n qubits:");
    Steps->AddText("H^{#otimesn}|0#GT^{#otimesn}");
    Steps->AddText("Apply a sign shift with the oracle U_{f} ");((TText*)Steps->GetListOfLines()->Last())->SetTextColor(kOrange+1);
    Steps->AddText("Apply the Diffusion transform D");
    Steps->AddText("Measure the states (qubits)");

    StepGates = new TPaveText(0.2,0.1,0.8,0.9);
    StepGates->SetTextAlign(12);
    StepGates->SetShadowColor(0);
    StepGates->SetFillColor(0);
    StepGates->SetLineColor(0);
    StepGates->SetTextSize(0.05);
    StepGates->AddText("Apply oracle U_{f}:");
    StepGates->AddText("#color[22]{   U_{f} = #lower[0.2]{#scale[4.5]{(}} #splitline{#splitline{1-2f(0)}{   0}}{#splitline{#splitline{  #upoint}{#lower[-0.5]{  #upoint}}}{#splitline{#lower[-0.5]{  #upoint}}{#lower[-0.3]{   0}}}}    #splitline{#splitline{   0}{1-2f(1)}}{#splitline{#splitline{  #upoint}{#lower[-0.5]{  #upoint}}}{#splitline{#lower[-0.5]{  #upoint}}{#lower[-0.3]{   0}}}}   #3dots   #splitline{#splitline{   0}{   0}}{#splitline{#splitline{  #upoint}{#lower[-0.5]{  #upoint}}}{#splitline{#lower[-0.5]{  #upoint}}{#lower[-0.3]{1-2f(N)}}}} #lower[0.2]{#scale[4.5]{)}}}");
    StepGates->AddText("");
    StepGates->AddText("The oracle U_{f} produces a sign shift over the search element");
    
    double xf;
    double yf;
    if(k<N){
      xf=sqrt(ms[k]*ms[k]*(N-1));
      yf=ms[k-1];
    }
    else{
      xf=sqrt(ms[0]*ms[0]*(N-1));
      yf=ms[k-1];
    }
    SVector = new TArrow(0.0,0.0,xf,yf,0.015,"|>");
    SVector->SetFillColor(2);
    SVector->SetLineColor(2);
    
    double xinextg=0.4+0.16*(r-1)+0.04*(r+1*(r-1));
    double xfnextg=0.48+0.16*(r-1)+0.04*(r+1*(r-1));
    
    switch(n){
    case 2:
      fCanvas->cd(1);
      
      latex->SetTextSize(0.05);
      latex->DrawLatex(0.02,0.685,"|0#GT");
      latex->DrawLatex(0.02,0.285,"|0#GT");
      
      UfGate = new TPaveText(xinextg,0.224,xfnextg,0.774);
      UfGate->SetTextAlign(22);
      UfGate->SetShadowColor(0);
      UfGate->SetTextFont(11);
      UfGate->SetTextSize(0.07);
      UfGate->SetFillColor(22);
      UfGate->AddText("Uf");
      
      UfGate->Draw();
      //UfGate2->Draw();

      gCanvas->cd(1);
      UfGate->Draw();
      
      fCanvas->cd(2);
      
      Steps->Draw();

      gCanvas->cd(2);
      
      StepGates->Draw();
    
      fCanvas->cd(3)->Range(-1,-1,1,1);
      fCanvas->cd(3)->Clear();
    
      Axis1 = new TLine(0.0,0.0,0.9,0.0);
      Axis2 = new TLine(0.0,-0.9,0.0,0.85);
      //Axis3 = new TLine(0,0,-0.316,-0.589);
    
      latex->SetTextSize(0.06);
      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
      //latex->DrawLatex(xf-0.03,yf-0.12,"U_{f}|#varphi#GT");
    
      Axis1->Draw();
      Axis2->Draw();
      SVector->Draw();
      GV->Draw();

      gCanvas->cd(3)->Range(-1,-1,1,1);
      gCanvas->cd(3)->Clear();

      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    
      Axis1->Draw();
      Axis2->Draw();
      SVector->Draw();
      GV->Draw();
      
      fCanvas->cd(4);
    
      PEstados->Draw("AB");

      gCanvas->cd(4);
    
      PEstados->Draw("AB");
    
      fCanvas->Update();
      gCanvas->Update();
    
      break;
    case 3:
    
      fCanvas->cd(1);

      latex->SetTextSize(0.05);
      latex->DrawLatex(0.02,0.735,"|0#GT");
      latex->DrawLatex(0.02,0.485,"|0#GT");
      latex->DrawLatex(0.02,0.235,"|0#GT");

      UfGate = new TPaveText(xinextg,0.174,xfnextg,0.824);
      UfGate->SetTextAlign(22);
      UfGate->SetShadowColor(0);
      UfGate->SetTextFont(11);
      UfGate->SetTextSize(0.07);
      UfGate->SetFillColor(22);
      UfGate->AddText("Uf");
    
    
      UfGate->Draw();
      // UfGate2->Draw();
      // UfGate3->Draw();

      gCanvas->cd(1);
      UfGate->Draw();
    
      fCanvas->cd(2);
      fCanvas->cd(2)->Clear();
      Steps->Draw();

      gCanvas->cd(2);
      gCanvas->cd(2)->Clear();
      StepGates->Draw();
    
      fCanvas->cd(3)->Range(-1,-1,1,1);
      fCanvas->cd(3)->Clear();
      
      Axis1 = new TLine(0.0,0.0,0.9,0.0);
      Axis2 = new TLine(0.0,-0.9,0.0,0.85);
      //Axis3 = new TLine(0,0,-0.316,-0.589);
    
      latex->SetTextSize(0.06);
      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
      //latex->DrawLatex(xf-0.05,yf-0.05,"U_{f}|#varphi#GT");
    
      Axis1->Draw();
      Axis2->Draw();
      SVector->Draw();
      GV->Draw();

      gCanvas->cd(3)->Range(-1,-1,1,1);
      gCanvas->cd(3)->Clear();

      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    
      Axis1->Draw();
      Axis2->Draw();
      SVector->Draw();
      GV->Draw();

    
      fCanvas->cd(4);
    
      PEstados->Draw("AB");

      gCanvas->cd(4);
    
      PEstados->Draw("AB");
    
      fCanvas->Update();
      gCanvas->Update();
      
      break;
    case 4:
      fCanvas->cd(1);
    
      latex->SetTextSize(0.05);
      latex->DrawLatex(0.02,0.735,"|0#GT");
      latex->DrawLatex(0.02,0.535,"|0#GT");
      latex->DrawLatex(0.02,0.335,"|0#GT");
      latex->DrawLatex(0.02,0.135,"|0#GT");

      UfGate = new TPaveText(xinextg,0.074,xfnextg,0.824);
      UfGate->SetTextAlign(22);
      UfGate->SetShadowColor(0);
      UfGate->SetTextFont(11);
      UfGate->SetTextSize(0.07);
      UfGate->SetFillColor(22);
      UfGate->AddText("Uf");

      UfGate->Draw();
      // UfGate2->Draw();
      //UfGate3->Draw();
      //UfGate4->Draw();

      gCanvas->cd(1);
      UfGate->Draw();
    
      fCanvas->cd(2);
      fCanvas->cd(2)->Clear();
      Steps->Draw();

      gCanvas->cd(2);
      gCanvas->cd(2)->Clear();
      StepGates->Draw();
      
      fCanvas->cd(3)->Range(-1,-1,1,1);
      fCanvas->cd(3)->Clear();
      
      Axis1 = new TLine(0.0,0.0,0.9,0.0);
      Axis2 = new TLine(0.0,-0.9,0.0,0.85);
      //Axis3 = new TLine(0,0,-0.316,-0.589);
    
      latex->SetTextSize(0.06);
      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
      //latex->DrawLatex(xf,yf,"U_{f}|#varphi#GT");
    
      Axis1->Draw();
      Axis2->Draw();
      SVector->Draw();
      GV->Draw();

      gCanvas->cd(3)->Range(-1,-1,1,1);
      gCanvas->cd(3)->Clear();

      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    
      Axis1->Draw();
      Axis2->Draw();
      SVector->Draw();
      GV->Draw();

    
      fCanvas->cd(4);
    
      fCanvas->SetTitle("Circuit");
      PEstados->Draw("AB");

      gCanvas->cd(4);
   
      PEstados->Draw("AB");
	
      fCanvas->Update();
      gCanvas->Update();
      
      break;
    }

    if(n>4){

      double xinextg=0.255+0.09*(r-1)+0.02*(r+1*(r-1));
      double xfnextg=0.30+0.09*(r-1)+0.02*(r+1*(r-1));

      fCanvas->cd(1)->SetPad(0.01,0.51,0.85,0.99);
      
      UfGate = new TPaveText(xinextg,0.074,xfnextg,0.824);
      UfGate->SetTextAlign(22);
      UfGate->SetShadowColor(0);
      UfGate->SetTextFont(11);
      UfGate->SetTextSize(0.07);
      UfGate->SetFillColor(22);
      UfGate->AddText("Uf");
    

      UfGate->Draw();

      gCanvas->cd(1)->SetPad(0.01,0.51,0.85,0.99);
      UfGate->Draw();
      
      fCanvas->cd(2)->SetPad(0.86,0.51,0.99,0.99);
      fCanvas->cd(2)->Clear();

      gCanvas->cd(2)->SetPad(0.86,0.51,0.99,0.99);
      gCanvas->cd(2)->Clear();
    
      fCanvas->cd(3)->Range(-1,-1,1,1);
      fCanvas->cd(3)->Clear();
      
      Axis1 = new TLine(0.0,0.0,0.9,0.0);
      Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    
      latex->SetTextSize(0.06);
      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
      //latex->DrawLatex(xf,yf,"U_{f}|#varphi#GT");
    
      Axis1->Draw();
      Axis2->Draw();
      SVector->Draw();
      GV->Draw();

      gCanvas->cd(3)->Range(-1,-1,1,1);
      gCanvas->cd(3)->Clear();

      latex->DrawLatex(-0.07,0.89,"|#beta#GT");
      latex->DrawLatex(0.92,-0.02,"|#alpha#GT");
    
      Axis1->Draw();
      Axis2->Draw();
      SVector->Draw();
      GV->Draw();
    
      fCanvas->cd(4);
    
      fCanvas->SetTitle("Circuit");
      PEstados->Draw("AB");

      gCanvas->cd(4);
    
      PEstados->Draw("AB");
	
      fCanvas->Update();
      gCanvas->Update();

    }

    half=1;
  }

  if(r==R){
    Chrono->TurnOff();
    previous->SetState(kButtonUp, kTRUE);
    next->SetState(kButtonUp, kTRUE);
  }
}

void MyMainFrame::Measure(){

  TEllipse *Arc, *Arc2, *Arc3, *Arc4;
  latex = new TLatex();
  
  fCanvas->Update();
  
  Steps = new TPaveText(0.2,0.1,0.8,0.9);
  Steps->SetTextAlign(12);
  Steps->SetShadowColor(0);
  Steps->SetFillColor(0);
  Steps->SetLineColor(0);
  Steps->SetTextSize(0.05);
  Steps->AddText("Initialize the n qubits to state |0#GT"); 
  Steps->AddText("Apply Hadamard Gate over the n qubits:");
  Steps->AddText("H^{#otimesn}|0#GT^{#otimesn}");
  Steps->AddText("Apply a sign shift with the oracle U_{f} ");
  Steps->AddText("Apply the Diffusion transform D");
  Steps->AddText("Measure the states (qubits)");((TText*)Steps->GetListOfLines()->Last())->SetTextColor(kOrange+1);

  StepGates = new TPaveText(0.2,0.1,0.8,0.9);
  StepGates->SetTextAlign(12);
  StepGates->SetShadowColor(0);
  StepGates->SetFillColor(0);
  StepGates->SetLineColor(0);
  StepGates->SetTextSize(0.05);
  StepGates->AddText("#color[50]{Measure the states (qubits)}");
  StepGates->AddText("When the qubits are measure the wavefunction is collapsed and");
  StepGates->AddText("as a result we obtain only one state, the probability of measuring");
  StepGates->AddText("any state is given by the square of the amplitude of said state");
  StepGates->AddText("");

  double xim=0.4+0.1585*(R-1)+0.04*(R+(R-1));
  double xfm=0.48+0.1585*(R-1)+0.04*(R+(R-1));
  double xmid = (xfm-xim)/2;

  switch(n){

  case 2:

    fCanvas->cd(1);
    
    MeasureG = new TPaveText(xim,0.624,xfm,0.774);
    MeasureG->SetTextAlign(22);
    MeasureG->SetShadowColor(0);
    MeasureG->SetTextFont(11);
    MeasureG->SetTextSize(0.07);
    MeasureG->SetFillColor(50);

    MeasureG2 = new TPaveText(xim,0.224,xfm,0.374);
    MeasureG2->SetTextAlign(22);
    MeasureG2->SetShadowColor(0);
    MeasureG2->SetTextFont(11);
    MeasureG2->SetTextSize(0.07);
    MeasureG2->SetFillColor(50);

    MeasureG->Draw();
    MeasureG2->Draw();

    Arc = new TEllipse(xim+xmid,0.68,0.0326,0.0588,0,180);
    Arc->SetNoEdges();
    Arc->SetLineWidth(2);
    Arc->SetFillColor(50);
    Arc->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.685,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.66,"#upoint");
    
    Arc2 = new TEllipse(xim+xmid,0.28,0.0326,0.0588,0,180);
    Arc2->SetNoEdges();
    Arc2->SetLineWidth(2);
    Arc2->SetFillColor(50);
    Arc2->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.285,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.26,"#upoint");

    gCanvas->cd(1);
    MeasureG->Draw();
    MeasureG2->Draw();
    
    Arc->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.685,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.66,"#upoint");
    
    Arc2->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.285,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.26,"#upoint");
    
    fCanvas->cd(2)->Clear();
    Steps->Draw();

    gCanvas->cd(2)->Clear();
    StepGates->Draw();
    
    fCanvas->Update();
    gCanvas->Update();
    
    break;

  case 3:

    fCanvas->cd(1);

    MeasureG = new TPaveText(xim,0.674,xfm,0.824);
    MeasureG->SetTextAlign(22);
    MeasureG->SetShadowColor(0);
    MeasureG->SetTextFont(11);
    MeasureG->SetTextSize(0.07);
    MeasureG->SetFillColor(50);
    
    MeasureG2 = new TPaveText(xim,0.424,xfm,0.574);
    MeasureG2->SetTextAlign(22);
    MeasureG2->SetShadowColor(0);
    MeasureG2->SetTextFont(11);
    MeasureG2->SetTextSize(0.07);
    MeasureG2->SetFillColor(50);
    
    MeasureG3 = new TPaveText(xim,0.174,xfm,0.324);
    MeasureG3->SetTextAlign(22);
    MeasureG3->SetShadowColor(0);
    MeasureG3->SetTextFont(11);
    MeasureG3->SetTextSize(0.07);
    MeasureG3->SetFillColor(50);
    
    MeasureG->Draw();
    MeasureG2->Draw();
    MeasureG3->Draw();

    Arc = new TEllipse(xim+xmid,0.73,0.0326,0.0588,0,180);
    Arc->SetNoEdges();
    Arc->SetLineWidth(2);
    Arc->SetFillColor(50);
    Arc->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.735,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.71,"#upoint");
    
    Arc2 = new TEllipse(xim+xmid,0.48,0.0326,0.0588,0,180);
    Arc2->SetNoEdges();
    Arc2->SetLineWidth(2);
    Arc2->SetFillColor(50);
    Arc2->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.485,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.46,"#upoint");

    Arc3 = new TEllipse(xim+xmid,0.23,0.0326,0.0588,0,180);
    Arc3->SetNoEdges();
    Arc3->SetLineWidth(2);
    Arc3->SetFillColor(50);
    Arc3->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.235,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.21,"#upoint");
    
    gCanvas->cd(1);
    MeasureG->Draw();
    MeasureG2->Draw();
    MeasureG3->Draw();

    Arc->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.735,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.71,"#upoint");

    Arc2->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.485,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.46,"#upoint");

    Arc3->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.235,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.21,"#upoint");
    
    fCanvas->cd(2)->Clear();
    Steps->Draw();

    gCanvas->cd(2)->Clear();
    StepGates->Draw();
    
    fCanvas->Update();
    gCanvas->Update();

    break;
    
  case 4:

    fCanvas->cd(1);

    MeasureG = new TPaveText(xim,0.674,xfm,0.824);
    MeasureG->SetTextAlign(22);
    MeasureG->SetShadowColor(0);
    MeasureG->SetFillColor(50);
    
    MeasureG2 = new TPaveText(xim,0.474,xfm,0.624);
    MeasureG2->SetTextAlign(22);
    MeasureG2->SetShadowColor(0);
    MeasureG2->SetFillColor(50);
    
    MeasureG3 = new TPaveText(xim,0.274,xfm,0.424);
    MeasureG3->SetTextAlign(22);
    MeasureG3->SetShadowColor(0);
    MeasureG3->SetTextFont(11);
    MeasureG3->SetTextSize(0.07);
    MeasureG3->SetFillColor(50);
    
    MeasureG4 = new TPaveText(xim,0.074,xfm,0.224);
    MeasureG4->SetTextAlign(22);
    MeasureG4->SetShadowColor(0);
    MeasureG4->SetFillColor(50);
    
    MeasureG->Draw();
    MeasureG2->Draw();
    MeasureG3->Draw();
    MeasureG4->Draw();
    
    Arc = new TEllipse(xim+xmid,0.73,0.0326,0.0588,0,180);
    Arc->SetNoEdges();
    Arc->SetLineWidth(2);
    Arc->SetFillColor(50);
    Arc->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.735,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.71,"#upoint");
    
    Arc2 = new TEllipse(xim+xmid,0.53,0.0326,0.0588,0,180);
    Arc2->SetNoEdges();
    Arc2->SetLineWidth(2);
    Arc2->SetFillColor(50);
    Arc2->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.535,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.51,"#upoint");

    Arc3 = new TEllipse(xim+xmid,0.33,0.0326,0.0588,0,180);
    Arc3->SetNoEdges();
    Arc3->SetLineWidth(2);
    Arc3->SetFillColor(50);
    Arc3->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.34,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.31,"#upoint");

    Arc4 = new TEllipse(xim+xmid,0.13,0.0326,0.0588,0,180);
    Arc4->SetNoEdges();
    Arc4->SetLineWidth(2);
    Arc4->SetFillColor(50);
    Arc4->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.14,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.11,"#upoint");

    gCanvas->cd(1);
    MeasureG->Draw();
    MeasureG2->Draw();
    MeasureG3->Draw();
    MeasureG4->Draw();

    Arc->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.735,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.71,"#upoint");

    Arc2->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.535,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.51,"#upoint");

    Arc3->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.34,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.31,"#upoint");

    Arc4->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.14,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.0058,0.11,"#upoint");

    fCanvas->cd(2)->Clear();
    Steps->Draw();

    gCanvas->cd(2)->Clear();
    StepGates->Draw();
    
    fCanvas->Update();
    gCanvas->Update();

    break;
    
  }

  if(n>4){

     double xim=0.255+0.09*(R-1)+0.02*(R+1*(R-1));
     double xfm=0.30+0.09*(R-1)+0.02*(R+1*(R-1));
     double xmid = (xfm-xim)/2;
    
    fCanvas->cd(1);

    MeasureG = new TPaveText(xim,0.674,xfm,0.824);
    MeasureG->SetTextAlign(22);
    MeasureG->SetShadowColor(0);
    MeasureG->SetFillColor(50);
    
    MeasureG2 = new TPaveText(xim,0.074,xfm,0.224);
    MeasureG2->SetTextAlign(22);
    MeasureG2->SetShadowColor(0);
    MeasureG2->SetFillColor(50);
    
    MeasureG->Draw();
    MeasureG2->Draw();
    
    Arc = new TEllipse(xim+xmid,0.73,0.0196,0.0588,0,180);
    Arc->SetNoEdges();
    Arc->SetLineWidth(2);
    Arc->SetFillColor(50);
    Arc->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.735,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.003,0.71,"#upoint");
    

    Arc3 = new TEllipse(xim+xmid,0.13,0.0196,0.0588,0,180);
    Arc3->SetNoEdges();
    Arc3->SetLineWidth(2);
    Arc3->SetFillColor(50);
    Arc3->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.14,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.003,0.11,"#upoint");

    gCanvas->cd(1);
    MeasureG->Draw();
    MeasureG2->Draw();

    Arc->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.735,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.003,0.71,"#upoint");

    Arc3->Draw();
    latex->SetTextSize(0.08);
    latex->SetTextAngle(-35);
    latex->DrawLatex(xim+xmid,0.14,"#void8");
    latex->SetTextAngle(0);
    latex->SetTextSize(0.1);
    latex->DrawLatex(xim+xmid-0.003,0.11,"#upoint");
    
    fCanvas->Update();
    gCanvas->Update();
    
  }
  
  if(R==1){
    previous->Disconnect("Clicked()");
    previous->Connect("Clicked()","MyMainFrame",this,"Diffuser()");
  }
  
}

/*-----------------------------------------------------------------------------
-------------------------------------------------------------------------------
--------------------------BASIC CONCEPTS FUNCTIONS-----------------------------
-------------------------------------------------------------------------------
-----------------------------------------------------------------------------*/

void MyMainFrame::GateConstruct(){
  fCanvas = MCanvas->GetCanvas();
  fCanvas->Clear();
  fCanvas->Divide(3,1);
  fCanvas->Update();

  gCanvas = MCanvas2->GetCanvas();
  gCanvas->Clear();
  gCanvas->Divide(3,1);
  gCanvas->Update();

  latex = new TLatex();

  TText *oracle = new TText(0.5,0.95,"ORACLE Uf");
  oracle->SetTextAlign(22);
  oracle->SetTextColor(kBlue+2);
  oracle->SetTextFont(32);
  oracle->SetTextSize(0.07);
  
  fCanvas->cd(1);
  oracle->Draw();
  latex->SetTextSize(0.06);
  latex->DrawLatex(0.02,0.8,"#font[11]{The oracle U_{f} #color[50]{implements a phase shift to}}");
  latex->DrawLatex(0.02,0.75,"#font[11]{#color[50]{the state we are searching for}, lets call this}");
  latex->DrawLatex(0.02,0.7,"#font[11]{state #color[4]{K}. In this context we can define:}");
  latex->DrawLatex(0.02,0.62,"#font[11]{U_{f}=(-1)^{f(x)} where f(x) is given by:}");
  latex->DrawLatex(0.3,0.48,"#font[11]{f(x)=}");
  latex->SetTextSize(0.15);
  latex->DrawLatex(0.4,0.46,"#font[11]{{}");
  latex->SetTextSize(0.06);
  latex->DrawLatex(0.45,0.50,"#font[11]{0 if x #neq #color[4]{K}}");
  latex->DrawLatex(0.43,0.45,"#font[11]{1 if x = #color[4]{K}}");
  latex->DrawLatex(0.02,0.35,"#font[11]{In a way that, when the operator (gate) is applied}");
  latex->DrawLatex(0.02,0.30,"#font[11]{to the given state, all states will remain the}");
  latex->DrawLatex(0.02,0.25,"#font[11]{same, #color[50]{except for the} #color[4]{K} #color[50]{state which will be}}");
  latex->DrawLatex(0.02,0.2,"#font[11]{#color[50]{flipped to a negative phase}, diferenciating}");
  latex->DrawLatex(0.02,0.15,"#font[11]{it from the rest.}");

  fCanvas->cd(2);

  latex->DrawLatex(0.02,0.85,"#font[10]{Lets suppose the next superposition state:}");
  latex->DrawLatex(0.02,0.75,"#font[11]{|#varphi#GT=#frac{1}{#sqrt{N}}(a|00#upoint#upoint#upoint 0#GT + b|00#upoint#upoint#upoint 1#GT + #upoint#upoint#upoint}");
  latex->DrawLatex(0.02,0.66,"#font[11]{+i|#color[4]{K}#GT + #upoint#upoint#upoint + v|11#upoint#upoint#upoint 1#GT)}");
  latex->DrawLatex(0.02,0.58,"#font[11]{Applying the U_{f} gate, to the state, gives as}");
   latex->DrawLatex(0.02,0.53,"#font[11]{a result:}");
  latex->DrawLatex(0.02,0.46,"#font[11]{(-1)^{f(x)}|#varphi#GT=#frac{1}{#sqrt{N}}(a|00#upoint#upoint#upoint 0#GT + b|00#upoint#upoint#upoint 1#GT +}");
  latex->DrawLatex(0.02,0.37,"#font[11]{#upoint#upoint#upoint #color[2]{-}i|#color[4]{K}#GT + #upoint#upoint#upoint + v|11#upoint#upoint#upoint 1#GT)}");
  latex->DrawLatex(0.02,0.30, "#font[11]{The U_{f} gate can be written in the matrix form as:}");
  latex->DrawLatex(0.0,0.15,"#font[11]{U_{f}=}");
  latex->SetTextSize(0.33);
  latex->DrawLatex(0.105,0.11,"#font[132]{(}");
  latex->DrawLatex(0.913,0.11,"#font[132]{)}");
  latex->SetTextSize(0.06);
  latex->DrawLatex(0.2,0.22,"#font[11]{1-2f(0)         0       #upoint#upoint#upoint        0}");
  latex->DrawLatex(0.215,0.18,"#font[11]{  0           1-2f(1)    #upoint#upoint#upoint        0}");
  latex->SetTextAngle(90);
  latex->DrawLatex(0.26,0.13,"#font[11]{#upoint#upoint#upoint}");
  latex->DrawLatex(0.50,0.13,"#font[11]{#upoint#upoint#upoint}");
  latex->DrawLatex(0.835,0.13,"#font[11]{#upoint#upoint#upoint}");
  latex->SetTextAngle(158);
  latex->DrawLatex(0.71,0.14,"#font[11]{#upoint #upoint #upoint}");
  latex->SetTextAngle(0);
  latex->DrawLatex(0.19,0.075,"#font[11]{   0               0       #upoint#upoint#upoint    1-2f(N)}");


  fCanvas->cd(3);

  latex->DrawLatex(0.02,0.85,"#font[10]{We can, aswell, see that #color[50]{the U_{f} operator}}");
  latex->DrawLatex(0.02,0.78,"#font[10]{#color[50]{is equivalent} to the operation #color[51]{I-2|#color[4]{K}#GT#LT#color[4]{K}|}}");
  latex->DrawLatex(0.20,0.69,"#font[10]{#color[51]{(I-2|#color[4]{K}#GT#LT#color[4]{K}|)} |#varphi#GT=I|#varphi#GT-2|#color[4]{K}#GT#LT#color[4]{K}|#varphi#GT}");
  latex->DrawLatex(0.02,0.61,"#font[10]{where #LTk|#varphi#GT is #color[50]{the inside product}, which is}");
 latex->DrawLatex(0.02,0.56,"#font[10]{equal to 0 if #varphi#neq#color[4]{K} an 1 if #varphi=#color[4]{K}}");
 latex->DrawLatex(0.02,0.51,"#font[10]{Lets rewrite the |#varphi#GT state as |#phi#GT+|#color[4]{K}#GT}");
  latex->DrawLatex(0.20,0.42,"#font[10]{=I(|#phi#GT+|#color[4]{K}#GT)-2|#color[4]{K}#GT#LT#color[4]{K}|(|#phi#GT+|#color[4]{K}#GT)}");
  latex->DrawLatex(0.20,0.34,"#font[10]{=(|#phi#GT+|#color[4]{K}#GT)-2|#color[4]{K}#GT(#LT#color[4]{K}|#phi#GT+#LT#color[4]{K}|#color[4]{K}#GT)}");
  latex->DrawLatex(0.20,0.26,"#font[10]{=(|#phi#GT+|#color[4]{K}#GT)-2|#color[4]{K}#GT}");
  latex->DrawLatex(0.20,0.18,"#font[10]{=|#phi#GT#color[2]{-}|#color[4]{K}#GT}");
  latex->DrawLatex(0.02,0.11,"#font[10]{We can see that the |#color[4]{K}#GT state sign was}");
   latex->DrawLatex(0.02,0.06,"#font[11]{flipped}");
  
  fCanvas->Update();

  gCanvas->cd(1);
  oracle->Draw();
  latex->SetTextSize(0.06);
  latex->DrawLatex(0.02,0.8,"#font[11]{The oracle U_{f} #color[50]{implements a phase shift to}}");
  latex->DrawLatex(0.02,0.75,"#font[11]{#color[50]{the state we are searching for}, lets call this}");
  latex->DrawLatex(0.02,0.7,"#font[11]{state #color[4]{K}. In this context we can define:}");
  latex->DrawLatex(0.02,0.62,"#font[11]{U_{f}=(-1)^{f(x)} where f(x) is given by:}");
  latex->DrawLatex(0.3,0.48,"#font[11]{f(x)=}");
  latex->SetTextSize(0.15);
  latex->DrawLatex(0.4,0.46,"#font[11]{{}");
  latex->SetTextSize(0.06);
  latex->DrawLatex(0.45,0.50,"#font[11]{0 if x #neq #color[4]{K}}");
  latex->DrawLatex(0.43,0.45,"#font[11]{1 if x = #color[4]{K}}");
  latex->DrawLatex(0.02,0.35,"#font[11]{In a way that, when the operator (gate) is applied}");
  latex->DrawLatex(0.02,0.30,"#font[11]{to the given state, all states will remain the}");
  latex->DrawLatex(0.02,0.25,"#font[11]{same, #color[50]{except for the} #color[4]{K} #color[50]{state which will be}}");
  latex->DrawLatex(0.02,0.2,"#font[11]{#color[50]{flipped to a negative phase}, diferenciating}");
  latex->DrawLatex(0.02,0.15,"#font[11]{it from the rest.}");

  gCanvas->cd(2);

  latex->DrawLatex(0.02,0.85,"#font[10]{Lets suppose the next superposition state:}");
  latex->DrawLatex(0.02,0.75,"#font[11]{|#varphi#GT=#frac{1}{#sqrt{N}}(a|00#upoint#upoint#upoint 0#GT + b|00#upoint#upoint#upoint 1#GT + #upoint#upoint#upoint}");
  latex->DrawLatex(0.02,0.66,"#font[11]{+i|#color[4]{K}#GT + #upoint#upoint#upoint + v|11#upoint#upoint#upoint 1#GT)}");
  latex->DrawLatex(0.02,0.58,"#font[11]{Applying the U_{f} gate, to the state, gives as}");
   latex->DrawLatex(0.02,0.53,"#font[11]{a result:}");
  latex->DrawLatex(0.02,0.46,"#font[11]{(-1)^{f(x)}|#varphi#GT=#frac{1}{#sqrt{N}}(a|00#upoint#upoint#upoint 0#GT + b|00#upoint#upoint#upoint 1#GT +}");
  latex->DrawLatex(0.02,0.37,"#font[11]{#upoint#upoint#upoint #color[2]{-}i|#color[4]{K}#GT + #upoint#upoint#upoint + v|11#upoint#upoint#upoint 1#GT)}");
  latex->DrawLatex(0.02,0.30, "#font[11]{The U_{f} gate can be written in the matrix form as:}");
  latex->DrawLatex(0.0,0.15,"#font[11]{U_{f}=}");
  latex->SetTextSize(0.33);
  latex->DrawLatex(0.105,0.11,"#font[132]{(}");
  latex->DrawLatex(0.913,0.11,"#font[132]{)}");
  latex->SetTextSize(0.06);
  latex->DrawLatex(0.2,0.22,"#font[11]{1-2f(0)         0       #upoint#upoint#upoint        0}");
  latex->DrawLatex(0.215,0.18,"#font[11]{  0           1-2f(1)    #upoint#upoint#upoint        0}");
  latex->SetTextAngle(90);
  latex->DrawLatex(0.26,0.13,"#font[11]{#upoint#upoint#upoint}");
  latex->DrawLatex(0.50,0.13,"#font[11]{#upoint#upoint#upoint}");
  latex->DrawLatex(0.835,0.13,"#font[11]{#upoint#upoint#upoint}");
  latex->SetTextAngle(158);
  latex->DrawLatex(0.71,0.14,"#font[11]{#upoint #upoint #upoint}");
  latex->SetTextAngle(0);
  latex->DrawLatex(0.19,0.075,"#font[11]{   0               0       #upoint#upoint#upoint    1-2f(N)}");


  gCanvas->cd(3);

  latex->DrawLatex(0.02,0.85,"#font[10]{We can, aswell, see that #color[50]{the U_{f} operator}}");
  latex->DrawLatex(0.02,0.78,"#font[10]{#color[50]{is equivalent} to the operation #color[51]{I-2|#color[4]{K}#GT#LT#color[4]{K}|}}");
  latex->DrawLatex(0.20,0.69,"#font[10]{#color[51]{(I-2|#color[4]{K}#GT#LT#color[4]{K}|)} |#varphi#GT=I|#varphi#GT-2|#color[4]{K}#GT#LT#color[4]{K}|#varphi#GT}");
  latex->DrawLatex(0.02,0.61,"#font[10]{where #LTk|#varphi#GT is #color[50]{the inside product}, which is}");
 latex->DrawLatex(0.02,0.56,"#font[10]{equal to 0 if #varphi#neq#color[4]{K} an 1 if #varphi=#color[4]{K}}");
 latex->DrawLatex(0.02,0.51,"#font[10]{Lets rewrite the |#varphi#GT state as |#phi#GT+|#color[4]{K}#GT}");
  latex->DrawLatex(0.20,0.42,"#font[10]{=I(|#phi#GT+|#color[4]{K}#GT)-2|#color[4]{K}#GT#LT#color[4]{K}|(|#phi#GT+|#color[4]{K}#GT)}");
  latex->DrawLatex(0.20,0.34,"#font[10]{=(|#phi#GT+|#color[4]{K}#GT)-2|#color[4]{K}#GT(#LT#color[4]{K}|#phi#GT+#LT#color[4]{K}|#color[4]{K}#GT)}");
  latex->DrawLatex(0.20,0.26,"#font[10]{=(|#phi#GT+|#color[4]{K}#GT)-2|#color[4]{K}#GT}");
  latex->DrawLatex(0.20,0.18,"#font[10]{=|#phi#GT#color[2]{-}|#color[4]{K}#GT}");
  latex->DrawLatex(0.02,0.11,"#font[10]{We can see that the |#color[4]{K}#GT state sign was}");
  latex->DrawLatex(0.02,0.06,"#font[11]{flipped}");
  gCanvas->Update();
  
}

void MyMainFrame::DifgateDef(){

  fCanvas = MCanvas->GetCanvas();
  fCanvas->Clear();
  fCanvas->Divide(3,1);

  latex = new TLatex();

  TText *difusser = new TText(0.5,0.97,"DIFFUSER OPERATOR");
  difusser->SetTextAlign(22);
  difusser->SetTextColor(kBlue+2);
  difusser->SetTextFont(32);
  difusser->SetTextSize(0.07);

  fCanvas->cd(1);

  difusser->Draw();
  
  latex->SetTextSize(0.05);
  latex->DrawLatex(0.02,0.88,"#font[11]{The diffuser operator is defined, by Grover, in its}");
  latex->DrawLatex(0.02,0.83,"#font[11]{matrix form, as follow:}");
  latex->DrawLatex(0.2,0.75,"#font[11]{#color[2]{D_{ij}=#frac{2}{N}  with i#neqj} and #color[4]{D_{ii}=-1+#frac{2}{N}}}");
  latex->DrawLatex(0.02,0.68,"#font[11]{Giving as result the matrix:}");
  latex->DrawLatex(0.02,0.44,"#font[11]{D=}");
  latex->SetTextSize(0.41);
  latex->DrawLatex(0.09,0.39,"#font[11]{(}");
  latex->DrawLatex(0.85,0.39,"#font[11]{)}");
  latex->SetTextSize(0.05);
  latex->DrawLatex(0.22,0.52,"#font[11]{-1+#frac{2}{N}         #frac{2}{N}       #upoint#upoint#upoint      #frac{2}{N}}");
  latex->DrawLatex(0.225,0.46,"#font[11]{ #frac{2}{N}             -1+#frac{2}{N}      #upoint#upoint#upoint      #frac{2}{N}}");
  latex->SetTextAngle(90);
  latex->DrawLatex(0.27,0.40,"#font[11]{#upoint#upoint#upoint}");
  latex->DrawLatex(0.50,0.40,"#font[11]{#upoint#upoint#upoint}");
  latex->DrawLatex(0.81,0.40,"#font[11]{#upoint#upoint#upoint}");
  latex->SetTextAngle(158);
  latex->DrawLatex(0.68,0.40,"#font[11]{#upoint #upoint #upoint}");
  latex->SetTextAngle(0);
  latex->DrawLatex(0.20,0.35,"#font[11]{  #frac{2}{N}          #frac{2}{N}       #upoint#upoint#upoint       -1+#frac{2}{N}}");
  latex->DrawLatex(0.02,0.13,"#font[11]{Where we can see #color[50]{only the diagonal elements are}}");
  latex->DrawLatex(0.02,0.08,"#font[11]{#color[50]{different from the others}. Watching the matrix}");
  latex->DrawLatex(0.02,0.03,"#font[11]{obtained, we can notice this could be rewritten}");

  fCanvas->cd(2);
  latex->DrawLatex(0.02,0.9,"#font[11]{as the sum of two matrices: #color[9]{-I+#frac{2}{N}J} where #color[50]{I is}}");
  latex->DrawLatex(0.02,0.83,"#font[11]{#color[50]{the identity matrix and J is an all ones matrix.}}");
  latex->DrawLatex(0.02,0.77,"#font[11]{The diffuser operator can also be rewritten as the}");
  latex->DrawLatex(0.02,0.71,"#font[11]{operation #color[51]{H^{#otimesn}RH^{#otimesn}} or #color[51]{H^{#otimesn}(2|0#GT^{#otimesn}#LT0|^{#otimesn}-I_{n})H^{#otimesn}}}");
  latex->DrawLatex(0.02,0.65,"#font[11]{Where #color[50]{H is the Hadamard gate} and #color[50]{R is -I except}}");
  latex->DrawLatex(0.02,0.59,"#font[11]{#color[50]{in R_{00}=1}. We can express the matrix R as two}");
  latex->DrawLatex(0.02,0.53,"#font[11]{different matrices #color[833]{R_{1}=-I} and #color[833]{R_{2,00}=2 and R_{2,ij}=0}}");
  latex->DrawLatex(0.02,0.47,"#font[11]{having as a result the operation #color[51]{H^{#otimesn}R_{1}H^{#otimesn}+}}");
  latex->DrawLatex(0.02,0.41,"#font[11]{#color[51]{H^{#otimesn}R_{2}H^{#otimesn}}. The first operation is easy, being:}");
  latex->DrawLatex(0.02,0.35,"#font[11]{#color[51]{H^{#otimesn}}#color[833]{(-I)}#color[51]{H^{#otimesn}}=#color[51]{H^{#otimesn}(#color[2]{-}H^{#otimesn})}=#color[808]{-I} #color[50]{Since H is unitary}}");
  latex->DrawLatex(0.02,0.29,"#font[11]{#color[50]{HH=I}. On the other hand, the second operation is}");
  latex->DrawLatex(0.02,0.23,"#font[11]{a little bit harder, for this one we will rewrite}");
  latex->DrawLatex(0.02,0.16,"#font[11]{the Hadamard gate as follow: #color[4]{H_{ij}=2^{-n/ 2}(-1)^{i #upoint j}}}");
  latex->DrawLatex(0.02,0.09,"#font[11]{#color[50]{Where i #upoint j is the bitwise dot product}, which}");
  latex->DrawLatex(0.02,0.03,"#font[11]{compares the amount of 1's the bit i shares with}");

  fCanvas->cd(3);
  
  latex->DrawLatex(0.02,0.9,"#font[11]{the bit j, #color[50]{if this amount is odd i #upoint j=1, else it's 0.}}");
  latex->DrawLatex(0.02,0.85,"#font[11]{In order to find this second operation we make an}");
  latex->DrawLatex(0.02,0.8,"#font[11]{standard matrix multiplication:}");
  latex->DrawLatex(0.02,0.71,"#font[11]{#color[51]{H^{#otimesn}}#color[833]{R_{2}}#color[51]{H^{#otimesn}}=#sum}#font[11]{#color[51]{H_{ab}}#color[833]{R_{2, bc}}#color[51]{H_{cd}}}");
  latex->DrawLatex(0.02,0.63,"#font[11]{given the definition of #color[833]{R_{2}} we obtain:}");
  latex->DrawLatex(0.02,0.54,"#font[11]{=#color[51]{H_{a0}}#color[833]{R_{2, 00}}#color[51]{H_{0d}}=2#frac{1}{2^{n/ 2}}(-1)^{a#upoint 0}#frac{1}{2^{n/ 2}}(-1)^{0#upoint d}}");
  latex->DrawLatex(0.02,0.46,"#font[11]{=#frac{2}{2^{n}}}");
  latex->DrawLatex(0.02,0.38,"#font[11]{But 2^{n}=N, so that #color[51]{H^{#otimesn}}#color[833]{R_{2}}#color[51]{H^{#otimesn}}=#color[808]{#frac{2}{N}}}");
  latex->DrawLatex(0.02,0.31,"#font[11]{Adding up the two obtained terms we get: #color[9]{-I+#frac{2}{N}}}");
  latex->DrawLatex(0.02,0.24,"#font[11]{which is the same operation as the diffuser gate}");
  latex->DrawLatex(0.02,0.18,"#font[11]{#color[50]{The diffuser operator is known as an invertion}}");
  latex->DrawLatex(0.02,0.12,"#font[11]{#color[50]{over the avarage}, this because it takes the state}");
  latex->DrawLatex(0.02,0.06,"#font[11]{previously shift and shifts it again with a #color[50]{bigger}}");
  latex->DrawLatex(0.02,0.01,"#font[11]{#color[50]{amplitude. Leaving the avarage unchanged.}}");
  
  fCanvas->Update();


}

void MyMainFrame::RDefinition(){
  fCanvas = MCanvas->GetCanvas();
  fCanvas->Clear();
  fCanvas->Divide(3,1);

  latex = new TLatex();

  fCanvas->cd(1);

  latex->SetTextSize(0.05);
  latex->DrawLatex(0.02,0.9,"#font[11]{#color[50]{The #color[4]{R} value dictates the amount of times it is}}");
  latex->DrawLatex(0.02,0.87,"#font[11]{#color[50]{necessary to perform the Grover protocol} (apply}");
  latex->DrawLatex(0.02,0.84,"#font[11]{the U_{f} and D gates) #color[50]{in order to have the closer}}");
  latex->DrawLatex(0.02,0.81,"#font[11]{#color[50]{to 1 probability of finding the desire state.}}");
  latex->DrawLatex(0.02,0.78,"#font[11]{In order to understand how to get the #color[4]{R} value let's}");
  latex->DrawLatex(0.02,0.75,"#font[11]{first apply the Grover algorithm to a #color[50]{2 qubit}}");
  latex->DrawLatex(0.02,0.72,"#font[11]{#color[50]{system, representing 4 elements:}}");
  latex->DrawLatex(0.17,0.67,"#font[11]{#color[51]{|#varphi#GT}=#color[2]{r_{0}}#color[51]{|00#GT}+#color[2]{r_{0}}#color[51]{|01#GT}+#color[833]{s_{0}}#color[51]{|10#GT}+#color[2]{r_{0}}#color[51]{|11#GT}}");
  latex->DrawLatex(0.02,0.62,"#font[11]{where the amplitude #color[2]{r_{0}} #color[50]{indicates the states that are}}");
  latex->DrawLatex(0.02,0.59,"#font[11]{#color[50]{no solution, and #color[833]{s_{0}} the ones that is} (the state the}");
  latex->DrawLatex(0.02,0.56,"#font[11]{algorithm is looking for). We are going to express}");
  latex->DrawLatex(0.02,0.53,"#font[11]{the Grover protocol by multiplying the U_{f} gate}");
  latex->DrawLatex(0.02,0.5,"#font[11]{by the D gate, obtaining:}");
  latex->DrawLatex(0.04,0.35,"#font[11]{#color[38]{DU_{f}}=}");
  latex->SetTextSize(0.43);
  latex->DrawLatex(0.18,0.31,"#font[11]{(}");
  latex->DrawLatex(0.71,0.31,"#font[11]{)}");
  latex->SetTextSize(0.05);
  latex->DrawLatex(0.28,0.43,"#font[11]{  -#frac{1}{2}      #frac{1}{2}       -#frac{1}{2}      #frac{1}{2}}");
  latex->DrawLatex(0.285,0.38,"#font[11]{ #frac{1}{2}       -#frac{1}{2}       -#frac{1}{2}      #frac{1}{2}}");
  latex->DrawLatex(0.255,0.33,"#font[11]{  #frac{1}{2}      #frac{1}{2}     #frac{1}{2}      #frac{1}{2}}");
  latex->DrawLatex(0.23,0.28,"#font[11]{   #frac{1}{2}      #frac{1}{2}       -#frac{1}{2}       -#frac{1}{2}}");
  latex->DrawLatex(0.02,0.21,"#font[11]{Applying this operation to the state |#varphi#GT we obtain:}");
  latex->DrawLatex(0.02,0.09,"#font[11]{#color[38]{DU_{f}}#color[51]{|#varphi#GT}=}");
  latex->SetTextSize(0.34);
  latex->DrawLatex(0.2,0.045,"#font[11]{(}");
  latex->DrawLatex(0.52,0.045,"#font[11]{)}");
  latex->SetTextSize(0.05);
  latex->DrawLatex(0.31,0.16,"#font[11]{1/2#color[2]{r_{0}}-1/2#color[833]{s_{0}}}");
  latex->DrawLatex(0.31,0.11,"#font[11]{1/2#color[2]{r_{0}}-1/2#color[833]{s_{0}}}");
  latex->DrawLatex(0.31,0.06,"#font[11]{3/2#color[2]{r_{0}}+1/2#color[833]{s_{0}}}");
  latex->DrawLatex(0.31,0.01,"#font[11]{1/2#color[2]{r_{0}}-1/2#color[833]{s_{0}}}");

  fCanvas->cd(2);
  
  latex->DrawLatex(0.02,0.95,"#font[11]{We can rewrite 1/2r_{0}-1/2s_{0} = #color[807]{r_{1}} and 3/2r_{0}+1/2s_{0}=#color[871]{s_{1}}}");
  latex->DrawLatex(0.02,0.895,"#font[11]{With the normalization constant being #frac{1}{#sqrt{3r_{1}^{2}+s_{1}^{2}}}}");
  latex->DrawLatex(0.02,0.835,"#font[11]{#color[50]{We can rewrite it by doing the next variable change:}}");
  latex->DrawLatex(0.02,0.8,"#font[11]{#color[434]{a_{i}}=#sqrt{3}#color[807]{r_{i}} and #color[634]{b_{i}}=#color[871]{s_{i}}. Obtaining:}");
  latex->DrawLatex(0.25,0.74,"#font[11]{#frac{#color[434]{a_{1}}}{#sqrt{3}} = #frac{1}{2#sqrt{3}}#color[434]{a_{0}} - #frac{1}{2}#color[634]{b_{0}}}");
  latex->DrawLatex(0.25,0.67,"#font[11]{b_{1} = #frac{3}{2#sqrt{3}}#color[434]{a_{0}} + #frac{1}{2}#color[634]{b_{0}}}");
  latex->DrawLatex(0.25,0.61,"#font[11]{#color[434]{a_{1}} = #frac{1}{2}#color[434]{a_{0}} - #frac{#sqrt{3}}{2}#color[634]{b_{0}}}");
  latex->DrawLatex(0.25,0.55,"#font[11]{#color[634]{b_{1}} = #frac{#sqrt{3}}{2}#color[434]{a_{0}} + #frac{1}{2}#color[636]{b_{0}}}");
  latex->DrawLatex(0.02,0.49,"#font[11]{Which we can rewrite in matrix form as:}");
  latex->SetTextSize(0.15);
  latex->DrawLatex(0.02,0.41,"#font[11]{(}");
  latex->DrawLatex(0.1,0.41,"#font[11]{)}");
  latex->DrawLatex(0.18,0.41,"#font[11]{(}");
  latex->DrawLatex(0.43,0.41,"#font[11]{)}");
  latex->DrawLatex(0.48,0.41,"#font[11]{(}");
  latex->DrawLatex(0.56,0.41,"#font[11]{)}");
  latex->SetTextSize(0.05);
  latex->DrawLatex(0.06,0.44,"#font[11]{#color[434]{ a_{1}}}");
  latex->DrawLatex(0.06,0.41,"#font[11]{#color[634]{b_{1}}}");
  latex->DrawLatex(0.15,0.42,"#font[11]{=}");
  latex->DrawLatex(0.23,0.44,"#font[11]{0.5    -#sqrt{3}/ 2}");
  latex->DrawLatex(0.23,0.41,"#font[11]{#sqrt{3}/ 2    0.5}");
  latex->DrawLatex(0.52,0.44,"#font[11]{#color[434]{ a_{0}}}");
  latex->DrawLatex(0.52,0.41,"#font[11]{#color[634]{b_{0}}}");
  latex->DrawLatex(0.02,0.35,"#font[11]{#color[50]{A transformation is performed in order to obtain}}");
  latex->DrawLatex(0.02,0.32,"#font[11]{#color[50]{the new state, this transformation is a rotation one}}");
  latex->DrawLatex(0.02,0.29,"#font[11]{as we can notice in the matrix written above.}");
  latex->DrawLatex(0.02,0.26,"#font[11]{The rotation matrix is defined as follow:}");
  latex->SetTextSize(0.15);
  latex->DrawLatex(0.14,0.19,"#font[11]{(}");
  latex->DrawLatex(0.47,0.19,"#font[11]{)}");
  latex->SetTextSize(0.05);
  latex->DrawLatex(0.02,0.205,"#font[11]{T(#theta)=}");
  latex->DrawLatex(0.18,0.22,"#font[11]{cos(#theta)     -sin(#theta)}");
  latex->DrawLatex(0.18,0.19,"#font[11]{sin(#theta)     cos(#theta)}");
  latex->DrawLatex(0.02,0.14,"#font[11]{We can rewrite the performed transformation in}");
  latex->DrawLatex(0.02,0.11,"#font[11]{terms of the rotation matrix, as follows:}");
  latex->SetTextSize(0.15);
  latex->DrawLatex(0.22,0.04,"#font[11]{(}");
  latex->DrawLatex(0.3,0.04,"#font[11]{)}");;
  latex->DrawLatex(0.51,0.04,"#font[11]{(}");
  latex->DrawLatex(0.59,0.04,"#font[11]{)}");
  latex->SetTextSize(0.05);
   latex->DrawLatex(0.26 ,0.07,"#font[11]{#color[434]{ a_{1}}}");
  latex->DrawLatex(0.26,0.04,"#font[11]{#color[634]{b_{1}}}");
  latex->DrawLatex(0.35,0.055,"#font[11]{=}");
  latex->DrawLatex(0.39,0.055,"#font[11]{R#left(#frac{#pi}{3}#right)}");
  latex->DrawLatex(0.55,0.07,"#font[11]{#color[434]{ a_{0}}}");
  latex->DrawLatex(0.55,0.04,"#font[11]{#color[634]{b_{0}}}");

  fCanvas->cd(3);
  latex->DrawLatex(0.02,0.95,"#font[11]{In this example our state consist of 3 non-solution}");
  latex->DrawLatex(0.02,0.91,"#font[11]{states and one that is solution, in order that the}");
  latex->DrawLatex(0.02,0.86,"#font[11]{initial amplitudes are #color[434]{a_{0}}= #sqrt{#frac{3}{4}} = #frac{#sqrt{3}}{2} and #color[634]{b_{0}}= #sqrt{#frac{1}{4}} = #frac{1}{2}}");
  latex->DrawLatex(0.02,0.82,"#font[11]{#color[50]{In terms of the angle, the initial amplitudes are:}}");
  latex->DrawLatex(0.02,0.78,"#font[11]{#color[434]{a_{0}}=cos#left(#frac{#pi}{6}#right) and #color[634]{b_{0}}=sin#left(#frac{#pi}{6}#right)}");
  latex->DrawLatex(0.02,0.74,"#font[11]{We can see that aour state was initially at a #frac{#pi}{6}}");
  latex->DrawLatex(0.02,0.70,"#font[11]{angle and was rotated #frac{#pi}{3}, obtaining #frac{#pi}{6} + #frac{#pi}{3} = #frac{#pi}{2}}");
  latex->DrawLatex(0.02,0.66,"#font[11]{which indicates our #color[50]{state was totally projected on}}");
  latex->DrawLatex(0.02,0.63,"#font[11]{#color[50]{the y-axis, which is the desire state}}");

  Axis1 = new TLine(0.0,0.4,0.7,0.4);
  Axis2 = new TLine(0.0,0.4,0.0,0.60);
  SVector = new TArrow(0.0,0.4,0.6,0.55,0.015,"|>");
  SVector->SetFillColor(2);
  SVector->SetLineColor(2);
  SVector2 = new TArrow(0.0,0.4,0.42,0.61,0.015,"|>");
  SVector2->SetFillColor(4);
  SVector2->SetLineColor(4);
  Axis1->Draw();
  Axis2->Draw();
  SVector->Draw();
  SVector2->Draw();

  latex->DrawLatex(0.25,0.415,"#font[11]{#frac{#theta}{2}}");
  latex->DrawLatex(0.16,0.45,"#font[11]{#theta}");

  latex->DrawLatex(0.02,0.36,"#font[11]{As shown, and in general, #color[50]{the initial state}}");
  latex->DrawLatex(0.02,0.33,"#font[11]{#color[50]{begins with an angle of #theta/2 and its rotated}}");
  latex->DrawLatex(0.02,0.30,"#font[11]{#color[50]{an angle #theta, this means that for every application}}");
  latex->DrawLatex(0.02,0.27,"#font[11]{#color[50]{of the Grover protocol the state will be rotated an}}");
  latex->DrawLatex(0.02,0.24,"#font[11]{#color[50]{amount of #theta}, we want to find the (#color[4]{R}) times needed}");
  latex->DrawLatex(0.02,0.20,"#font[11]{to rotate the state until #frac{#pi}{2}}");
  latex->DrawLatex(0.02,0.16,"#font[11]{#color[4]{R}#theta + #frac{#theta}{2} = #frac{#pi}{2}  By the approximation sin#theta #approx #theta}");
  latex->DrawLatex(0.02,0.12,"#font[11]{we can reeplace theta by the initial amplitude}");
  latex->DrawLatex(0.02,0.08,"#font[11]{#color[4]{R} #frac{2}{#sqrt{N}} + #frac{1}{#sqrt{N}} #approx #frac{#pi}{2}}");
  latex->DrawLatex(0.02,0.02,"#font[11]{#color[4]{R} #approx #frac{#pi}{4}#sqrt{N} - #frac{1}{2}}");
  latex->DrawLatex(0.02,0.04,"#font[11]{}");

  
  fCanvas->Update();
}

MyMainFrame::~MyMainFrame() {
   // Clean up used widgets: frames, buttons, layout hints
   MainF->Cleanup();
   delete MainF;
}


void Interface() {
  // Popup the GUI...
  new MyMainFrame(gClient->GetRoot(),850,450);
}

int main(int argc, char **argv) {
  TApplication theApp("App",&argc,argv);
  Interface();
  theApp.Run();
  return 0;
}
