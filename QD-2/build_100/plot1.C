{
    TCanvas *c1 = new TCanvas("c0", "c0", 18, 41, 700, 500);
        c1->SetTickx();
        c1->SetTicky();
        c1->SetLeftMargin(0.13);
        c1->SetRightMargin(0.07);
        c1->SetTopMargin(0.07);
        c1->SetBottomMargin(0.13);
        c1->SetGrid();
        c1->cd();
    //    c1->SetLogy();
    //    c1->SetLogx();
    
        int bins = 100;
    
        TH1D *h = new TH1D("", "pulse area", 50, -0.4e-9, 0.05e-9); //96 to somehting 
    
        h->SetStats(0);
        h->GetXaxis()->SetTitleSize(0.055);
        h->GetXaxis()->SetTitleOffset(1.15);
        h->SetLabelSize(0.055, "x");
        h->GetYaxis()->SetTitleSize(0.045);
        h->GetYaxis()->SetTitleOffset(0.95);
        h->SetLabelSize(0.055, "y");
        h->GetXaxis()->CenterTitle(1);
    //    h->SetLineColor(kOrange);
    //    h->SetFillColor(kOrange);
    //    TAxis *ay = h->GetYaxis();
    //    ay->SetMaxDigits(2);
    
        TFile *f = new TFile("output.root", "READ");
        TTree *t = (TTree*) f->FindObjectAny("Hits"); //the issue is that TTrea of output.root doesn't have a branch called time ig
    
    
        double primaryE;
        int hits;
        vector<double>* wavelength;
        vector<double>* time; 
    
        t->SetBranchAddress("primaryE", &primaryE);
        t->SetBranchAddress("hits", &hits);
        t->SetBranchAddress("wavelength", &wavelength);
        t->SetBranchAddress("time", &time);
        int hitCount;
        double chargeCount;
        double pulseArea = 0;
    
        for (int i=0; i<t->GetEntries(); i++) {
            t->GetEntry(i);
            if (hits>0) {
                hitCount = 0; // reset counter
                for (int j=0; j<hits; j++) {
                    //if ((*wavelength)[j]>300 && (*wavelength)[j]<600) {
                        //cout << wavelength[j] << endl;
                        hitCount++;
                        chargeCount = hitCount * 0.16 * 50;
    
                      /*  if (time && time->size() > 0) {
                            if (j + 1 < time->size()) {
                                double dt = ((*time)[j + 1] - (*time)[j]) / 10;
                                 pulseArea += chargeCount * dt;
                             }
                        } else {
                            std::cerr << "Error: 'time' vector is empty or not initialized!" << std::endl;
                        }
                        */
                   // }
                }
                std::cout << "pulseArea: " << pulseArea << std::endl;
                h->Fill(chargeCount); // assuming 20% efficiency, but can do this much more accurate by weighting hitcount in above loop using wavelength and a quantum efficiency curve
            }
        }
    
    
         h->Scale(1/h->Integral());
    
    
        h->Draw("hist");
        c1->SaveAs("original_output.png");
    }