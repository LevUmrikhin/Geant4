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

    TH1D *h = new TH1D("", "Hits", 50, 0, 500);

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
    TTree *t = (TTree*) f->FindObjectAny("Hits");


    double primaryE;
    int hits;
    vector<double>* wavelength;

    t->SetBranchAddress("primaryE", &primaryE);
    t->SetBranchAddress("hits", &hits);
    t->SetBranchAddress("wavelength", &wavelength);
    int hitCount;

    for (int i=0; i<t->GetEntries(); i++) {
        t->GetEntry(i);
        if (hits>0) {
            hitCount = 0; // reset counter
            for (int j=0; j<hits; j++) {
                if ((*wavelength)[j]>300 && (*wavelength)[j]<600) {
                    //cout << wavelength[j] << endl;
                    hitCount++;
                }
            }
            h->Fill(hitCount*0.2); // assuming 20% efficiency, but can do this much more accurate by weighting hitcount in above loop using wavelength and a quantum efficiency curve
        }
    }


//    h->Scale(1/h->Integral());


    h->Draw("hist");
}
