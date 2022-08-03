using namespace std;
using namespace TMath;
#include <fstream>
#include <sstream>
#include <string>

void pulser_results()
{

    Int_t Numberofchannels = 16;
    TH1D* h_CsI[Numberofchannels];
    TFile *input;
    TTree *AnalysisTree;
    char fname[200];
    char hname[+];
    char dname[50];
    char cname[20];
    char crname[20];
    ofstream peaks_proton_csv ("pulser_proton_peaks.csv");
    ofstream peaks_gamma_csv ("pulser_gamma_peaks.csv");
    peaks_proton_csv << "Voltage,Channel,Mean" << endl;
    peaks_gamma_csv << "Voltage,Channel,Mean" << endl;

    //LOOP for voltages values in proton range

    for (Int_t j = 5; j <=10; j++) 
    {
        if (j < 10)
        {
            sprintf(fname, "220707_pulser0%d_p.root",j);
        }
        else
        {
            sprintf(fname, "220707_pulser%d_p.root",j);
        }

        Int_t lim_inf = 50e3+(j-5)*10e3;
        Int_t lim_sup = 130e3+(j-5)*15e3;

        input = TFile::Open(fname);

        AnalysisTree = (TTree*)input->Get("AnalysisxTree");
        //Fazer loop sobre toldas as voltagens
        for (Int_t i = 0; i < Numberofchannels; i++)
        {
            sprintf(hname,"h_CsI[%d]", i);
            sprintf(dname,"l_CsI[%d]>>h_CsI[%d]", i,i);
            sprintf(cname,"l_CsI[%d]>0", i);
            sprintf(crname,"Crystal %d", i);


            h_CsI[i] = new TH1D(hname,crname, 80, lim_inf*10, lim_sup*10);
            AnalysisTree->Draw(dname, cname);
            TF1 *gaussfit1 = new TF1("gaussfit1","gaus", lim_inf*10,lim_sup*10);
            gaussfit1->SetParameter(0, h_CsI[i]->GetMaximum());
            h_CsI[i]->Fit("gaussfit1","R");

            if (j < 10)
            {
                cout << "Voltage: 0." << j << "    Crystal" << i+1 << "    Lim inf: " << lim_inf <<"     Lim sup: " << lim_sup <<  endl;

                peaks_gamma_csv << "0." << j << "," << i+1 << "," << gaussfit1->GetParameter(1) << endl;
            }
            else
            {
                cout << "Voltage: 1" << "    Crystal" << i+1<<  "    Lim inf: " << lim_inf <<"     Lim sup: " << lim_sup <<  endl;
                peaks_gamma_csv << "1" << "," << i+1 << "," << gaussfit1->GetParameter(1) << endl;
            }

        }
    }

    //LOOP for voltagens in gamma range
    for (Int_t j = 5; j <=10; j++) 
        {
            if (j < 10)
            {
                sprintf(fname, "220707_pulser0%d_g.root",j);
            }
            else
            {
                sprintf(fname, "220707_pulser%d_g.root",j);
            }
    
            Int_t lim_inf = 50e3+(j-5)*10e3;
            Int_t lim_sup = 130e3+(j-5)*15e3;

            input = TFile::Open(fname);

            AnalysisTree = (TTree*)input->Get("AnalysisxTree");
            //Fazer loop sobre toldas as voltagens
            for (Int_t i = 0; i < Numberofchannels; i++)
            {
                sprintf(hname,"h_CsI[%d]", i);
                sprintf(dname,"l_CsI[%d]>>h_CsI[%d]", i,i);
                sprintf(cname,"l_CsI[%d]>0", i);
                sprintf(crname,"Crystal %d", i);

                h_CsI[i] = new TH1D(hname,crname, 80, lim_inf, lim_sup);
                AnalysisTree->Draw(dname, cname);
                TF1 *gaussfit1 = new TF1("gaussfit1","gaus",lim_inf,lim_sup);
                gaussfit1->SetParameter(0, h_CsI[i]->GetMaximum());
                h_CsI[i]->Fit("gaussfit1","R");

                if (j < 10)
                {
                    cout << "Voltage: 0." << j << "    Crystal" << i+1 << "    Lim inf: " << lim_inf <<"     Lim sup: " << lim_sup <<  endl;
                    peaks_proton_csv << "0." << j << "," << i+1 << "," << gaussfit1->GetParameter(1) << endl;
                }
                else
                {
                    cout << "Voltage: 1" << "    Crystal" << i+1<<  "    Lim inf: " << lim_inf <<"     Lim sup: " << lim_sup <<  endl;
                    peaks_proton_csv << "1" << "," << i+1 << "," << gaussfit1->GetParameter(1) << endl;
                }

            }
        }

peaks_proton_csv.close();
peaks_gamma_csv.close();




}