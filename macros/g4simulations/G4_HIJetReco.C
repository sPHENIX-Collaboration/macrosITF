void HIJetReco(int verbosity = 0, bool do_flow = false ) {
  
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libjetbackground.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  JetReco *truthjetreco = new JetReco();
  TruthJetInput *tji = new TruthJetInput(Jet::PARTICLE);
  tji->add_embedding_flag( 0 ); // changes depending on signal vs. embedded
  truthjetreco->add_input(tji);
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.2),"AntiKt_Truth_r02");
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.3),"AntiKt_Truth_r03");
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.4),"AntiKt_Truth_r04");
  truthjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.5),"AntiKt_Truth_r05");
  truthjetreco->set_algo_node("ANTIKT");
  truthjetreco->set_input_node("TRUTH");
  truthjetreco->Verbosity(0);
  se->registerSubsystem(truthjetreco);

  RetowerCEMC *rcemc = new RetowerCEMC();
  rcemc->Verbosity( verbosity );
  se->registerSubsystem( rcemc );

  JetReco *towerjetreco = new JetReco();
  towerjetreco->add_input(new TowerJetInput(Jet::CEMC_TOWER_RETOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALIN_TOWER));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALOUT_TOWER));
  towerjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT,0.2),"AntiKt_Tower_HIRecoSeedsRaw_r02");
  towerjetreco->set_algo_node("ANTIKT");
  towerjetreco->set_input_node("TOWER");
  towerjetreco->Verbosity( verbosity );
  se->registerSubsystem(towerjetreco);

  DetermineTowerBackground *dtb = new DetermineTowerBackground();
  dtb->SetBackgroundOutputName("TowerBackground_Sub1");
  dtb->SetFlow( do_flow );
  dtb->SetSeedType( 0 );
  dtb->SetSeedJetD( 3 );
  dtb->Verbosity( verbosity );
  se->registerSubsystem( dtb );

  CopyAndSubtractJets *casj = new CopyAndSubtractJets();
  casj->SetFlowModulation( do_flow );
  casj->Verbosity( verbosity );
  se->registerSubsystem( casj );

  DetermineTowerBackground *dtb2 = new DetermineTowerBackground();
  dtb2->SetBackgroundOutputName("TowerBackground_Sub2");
  dtb2->SetFlow( do_flow );
  dtb2->SetSeedType( 1 );
  dtb2->SetSeedJetPt( 7 );
  dtb2->Verbosity( verbosity );
  se->registerSubsystem( dtb2 );
  
  SubtractTowers *st = new SubtractTowers();
  st->SetFlowModulation( do_flow );
  st->Verbosity( verbosity );
  se->registerSubsystem( st );

  JetReco *towerjetreco = new JetReco();
  towerjetreco->add_input(new TowerJetInput(Jet::CEMC_TOWER_SUB1));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALIN_TOWER_SUB1));
  towerjetreco->add_input(new TowerJetInput(Jet::HCALOUT_TOWER_SUB1));
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT,0.2,verbosity),"AntiKt_Tower_r02_Sub1");
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT,0.3,verbosity),"AntiKt_Tower_r03_Sub1");
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT,0.4,verbosity),"AntiKt_Tower_r04_Sub1");
  towerjetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT,0.5,verbosity),"AntiKt_Tower_r05_Sub1");
  towerjetreco->set_algo_node("ANTIKT");
  towerjetreco->set_input_node("TOWER");
  towerjetreco->Verbosity( verbosity );
  se->registerSubsystem(towerjetreco);

  return;

}  
