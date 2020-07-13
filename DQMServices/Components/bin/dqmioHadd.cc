#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <TTree.h>
#include <THashList.h>

#include <iostream>

// Utility function to check the consistency of the axis labels
// Taken from TH1::CheckBinLabels which is not public
static bool CheckBinLabels(const TAxis* a1, const TAxis* a2) {
  // Check that axis have same labels
  THashList* l1 = (const_cast<TAxis*>(a1))->GetLabels();
  THashList* l2 = (const_cast<TAxis*>(a2))->GetLabels();

  if (!l1 && !l2)
    return true;
  if (!l1 || !l2) {
    return false;
  }

  // Check now labels sizes  are the same
  if (l1->GetSize() != l2->GetSize()) {
    return false;
  }

  for (int i = 1; i <= a1->GetNbins(); ++i) {
    std::string_view label1 = a1->GetBinLabel(i);
    std::string_view label2 = a2->GetBinLabel(i);
    if (label1 != label2) {
      return false;
    }
  }

  return true;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> infiles = {
      "/eos/cms/store/user/srimanob/hlttdr/CMSSW_11_1_0/DQM/step1_WithValidation_inDQM_10.root",
      "/eos/cms/store/user/srimanob/hlttdr/CMSSW_11_1_0/DQM/step1_WithValidation_inDQM_11.root",
      "/eos/cms/store/user/srimanob/hlttdr/CMSSW_11_1_0/DQM/step1_WithValidation_inDQM_12.root",
      "/eos/cms/store/user/srimanob/hlttdr/CMSSW_11_1_0/DQM/step1_WithValidation_inDQM_13.root",
      "/eos/cms/store/user/srimanob/hlttdr/CMSSW_11_1_0/DQM/step1_WithValidation_inDQM_14.root",
      "/eos/cms/store/user/srimanob/hlttdr/CMSSW_11_1_0/DQM/step1_WithValidation_inDQM_15.root",
      "/eos/cms/store/user/srimanob/hlttdr/CMSSW_11_1_0/DQM/step1_WithValidation_inDQM_16.root",
      "/eos/cms/store/user/srimanob/hlttdr/CMSSW_11_1_0/DQM/step1_WithValidation_inDQM_17.root",
      "/eos/cms/store/user/srimanob/hlttdr/CMSSW_11_1_0/DQM/step1_WithValidation_inDQM_18.root",
      "/eos/cms/store/user/srimanob/hlttdr/CMSSW_11_1_0/DQM/step1_WithValidation_inDQM_19.root"};

  std::vector<TFile*> tfiles;

  for (auto f : infiles) {
    TFile* tf = TFile::Open(f.c_str());
    tfiles.push_back(tf);
  }

  std::vector<std::string> treenames = {// TODO: there are more.
                                        "TH1Fs",
                                        "TH2Fs",
                                        "TProfiles",
                                        "TProfile2Ds"};

  std::map<std::string, TH1*> store;
  int totobjects = 0;

  for (auto tname : treenames) {
    for (auto tf : tfiles) {
      std::cout << "Reading " << tname << " from " << tf->GetName() << "\n";
      std::string* m_fullName = nullptr;
      TH1* m_buffer = nullptr;
      uint32_t m_tag = 0;

      TTree* m_tree = dynamic_cast<TTree*>(tf->Get(tname.c_str()));
      m_tree->SetBranchAddress("FullName", &m_fullName);
      m_tree->SetBranchAddress("Flags", &m_tag);
      m_tree->SetBranchAddress("Value", &m_buffer);

      int min = 0;
      int max = m_tree->GetEntries();

      for (int i = min; i < max; i++) {
        m_tree->GetEntry(i);
        TH1*& existing = store[*m_fullName];
        TH1* original = existing;
        TH1* toAdd = m_buffer;

        if (!existing) {
          existing = dynamic_cast<TH1*>(m_buffer->Clone());
        } else {
          if (original->GetNbinsX() == toAdd->GetNbinsX() &&
              original->GetXaxis()->GetXmin() == toAdd->GetXaxis()->GetXmin() &&
              original->GetXaxis()->GetXmax() == toAdd->GetXaxis()->GetXmax() &&
              original->GetNbinsY() == toAdd->GetNbinsY() &&
              original->GetYaxis()->GetXmin() == toAdd->GetYaxis()->GetXmin() &&
              original->GetYaxis()->GetXmax() == toAdd->GetYaxis()->GetXmax() &&
              original->GetNbinsZ() == toAdd->GetNbinsZ() &&
              original->GetZaxis()->GetXmin() == toAdd->GetZaxis()->GetXmin() &&
              original->GetZaxis()->GetXmax() == toAdd->GetZaxis()->GetXmax() &&
              CheckBinLabels(original->GetXaxis(), toAdd->GetXaxis()) &&
              CheckBinLabels(original->GetYaxis(), toAdd->GetYaxis()) &&
              CheckBinLabels(original->GetZaxis(), toAdd->GetZaxis())) {
            original->Add(toAdd);
          } else {
            std::cout << "Found histograms with different axis limits or different labels '" << original->GetName()
                      << "' not merged.";
          }
          totobjects++;
        }
      }
    }
  }
  std::cout << "Read " << totobjects << " and merged them into " << store.size() << " objects.\n";

  return 0;
}
