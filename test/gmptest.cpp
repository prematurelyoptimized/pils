#include<iostream>
#include<cassert>
#include<gmpxx.h>
#include"../src/problem.hpp"
#include"../src/constraint.hpp"
#include"../src/variable.hpp"



void test_random_10x5(void) {
	Problem<mpz_class> problem;

	auto x1 = problem.addVariable("x1",8);
	auto x2 = problem.addVariable("x2",8);
	auto x3 = problem.addVariable("x3",8);
	auto x4 = problem.addVariable("x4",8);
	auto x5 = problem.addVariable("x5",8);

	auto obj = problem.addObjective("-10*x1+2*x2+17*x3-15*x4+11*x5");

	problem.addConstraint("0 <= 2*x1+2*x2+5*x4+3*x5-30");
	problem.addConstraint("0 >= -5*x1-x2+x3-x4-5*x5+35");
	problem.addConstraint("0 >= -4*x1-x2-x3-2*x4+4*x5");
	problem.addConstraint("0 <= -4*x1+3*x2-4*x3-x4+5*x5+4");
	problem.addConstraint("0 >= -3*x1-5*x2-2*x3+4*x4-3*x5+29");
	problem.addConstraint("0 >= x2+2*x4+x5-13");
	problem.addConstraint("0 >= -4*x1-2*x3-8*x4-x5");
	problem.addConstraint("0 >= 5*x1+2*x2-2*x3-2*x4+2*x5-8");
	problem.addConstraint("0 <= x1-2*x2+2*x3+3*x4+3*x5-30");
	problem.addConstraint("0 >= x1-2*x2-3*x3+5*x4-3*x5+17");

	SolutionStats res = problem.solve();

	std::cout << *(problem.tab) << '\n';

	assert(x1->value == 2);
	assert(x2->value == 2);
	assert(x3->value == 5);
	assert(x4->value == 3);
	assert(x5->value == 5);

	assert(obj->value == 79);
}

void test_random_40x20(void) {
	Problem<mpz_class> problem;

	auto x1 = problem.addVariable("x1",20);
	auto x2 = problem.addVariable("x2",20);
	auto x3 = problem.addVariable("x3",20);
	auto x4 = problem.addVariable("x4",20);
	auto x5 = problem.addVariable("x5",20);
	auto x6 = problem.addVariable("x6",20);
	auto x7 = problem.addVariable("x7",20);
	auto x8 = problem.addVariable("x8",20);
	auto x9 = problem.addVariable("x9",20);
	auto x10 = problem.addVariable("x10",20);
	auto x11 = problem.addVariable("x11",20);
	auto x12 = problem.addVariable("x12",20);
	auto x13 = problem.addVariable("x13",20);
	auto x14 = problem.addVariable("x14",20);
	auto x15 = problem.addVariable("x15",20);
	auto x16 = problem.addVariable("x16",20);
	auto x17 = problem.addVariable("x17",20);
	auto x18 = problem.addVariable("x18",20);
	auto x19 = problem.addVariable("x19",20);
	auto x20 = problem.addVariable("x20",20);
	
	problem.addConstraint("0<=-99*x1 +-59*x2 +54*x3 +-68*x4 +-105*x5 +3*x6 +-15*x7 +-8*x8 +-113*x9 +-288*x10 +-39*x11 +-115*x12 +33*x13 +97*x14 +200*x15 +-76*x16 +26*x17 +-87*x18 +73*x19 +-171*x20 +5719");
	problem.addConstraint("0<=58*x1 +-93*x2 +53*x3 +125*x4 +88*x5 +-35*x6 +-143*x7 +-99*x8 +116*x9 +15*x10 +32*x11 +-45*x12 +38*x13 +-82*x14 +109*x15 +-23*x16 +182*x17 +257*x18 +-208*x19 +58*x20 +-3026");
	problem.addConstraint("0<=15*x1 +-42*x2 +90*x3 +191*x4 +-16*x5 +-160*x6 +-38*x7 +219*x8 +-71*x9 +70*x10 +-167*x11 +27*x12 +-177*x13 +-68*x14 +12*x15 +46*x16 +146*x17 +-127*x18 +150*x19 +-166*x20 +-452");
	problem.addConstraint("0<=166*x1 +108*x2 +34*x3 +180*x4 +-106*x5 +95*x6 +123*x7 +-150*x8 +-16*x9 +-168*x10 +46*x11 +38*x12 +-53*x13 +-30*x14 +181*x15 +-113*x16 +-152*x17 +28*x18 +-286*x19 +71*x20 +-452");
	problem.addConstraint("0<=-147*x1 +-51*x2 +-203*x3 +51*x4 +126*x5 +-123*x6 +-19*x7 +110*x8 +115*x9 +64*x10 +55*x11 +-131*x12 +266*x13 +-185*x14 +-166*x15 +-86*x16 +133*x17 +-168*x18 +78*x19 +15*x20 +-269");
	problem.addConstraint("0<=-49*x1 +1*x2 +-135*x3 +-31*x4 +22*x5 +-38*x6 +-80*x7 +-1*x8 +75*x9 +-10*x10 +-29*x11 +97*x12 +-149*x13 +68*x14 +-141*x15+-32*x16 +-75*x17 +51*x18 +311*x19 +108*x20 +-940");
	problem.addConstraint("0<=64*x1 +-30*x2 +-36*x3 +-2*x4 +4*x5 +-13*x6 +-132*x7 +228*x8 +-46*x9 +-75*x10 +-125*x11 +147*x12 +-34*x13 +2*x14 +58*x15 +-69*x16 +-27*x17 +-71*x18 +72*x19 +-126*x20 +1620");
	problem.addConstraint("0<=51*x1 +21*x2 +-161*x3 +17*x4 +-147*x5 +28*x6 +-21*x7 +90*x8 +101*x9 +-36*x10 +-139*x11 +-154*x12 +-161*x13 +-57*x14 +-186*x15 +-69*x16 +-34*x17 +-65*x18 +-49*x19 +-42*x20 +5576");
	problem.addConstraint("0<=-100*x1 +-77*x2 +24*x3 +75*x4 +-149*x5 +166*x6 +-117*x7 +174*x8 +-9*x9 +-111*x10 +13*x11 +6*x12 +14*x13 +23*x14 +-216*x15 +64*x16 +158*x17 +104*x18 +38*x19 +-22*x20 +1692");
	problem.addConstraint("0<=81*x1 +-181*x2 +-81*x3 +-16*x4 +-48*x5 +18*x6 +199*x7 +133*x8 +-220*x9 +118*x10 +-236*x11 +-25*x12 +61*x13 +150*x14 +-81*x15 +102*x16 +-58*x17 +187*x18 +166*x19 +-33*x20 +-1720");
	problem.addConstraint("0<=-120*x1 +-32*x2 +76*x3 +-10*x4 +-5*x5 +2*x6 +-29*x7 +-69*x8 +-58*x9 +120*x10 +-108*x11 +191*x12 +212*x13 +-12*x14 +-126*x15 +-2*x16 +4*x17 +-21*x18 +-50*x19 +29*x20 +1236");
	problem.addConstraint("0<=-59*x1 +22*x2 +-110*x3 +-20*x4 +33*x5 +-4*x6 +13*x7 +-9*x8 +114*x9 +97*x10 +43*x11 +-62*x12 +88*x13 +-114*x14 +114*x15 +-31*x16 +111*x17 +5*x18 +-140*x19 +3*x20 +-1597");
	problem.addConstraint("0<=-71*x1 +38*x2 +-200*x3 +-28*x4 +75*x5 +38*x6 +9*x7 +131*x8 +39*x9 +3*x10 +142*x11 +70*x12 +201*x13 +-104*x14 +-51*x15 +177*x16 +-34*x17 +-180*x18 +-3*x19 +-203*x20 +1285");
	problem.addConstraint("0<=109*x1 +48*x2 +-215*x3 +1*x4 +7*x5 +133*x6 +8*x7 +53*x8 +-28*x9 +-183*x10 +-32*x11 +-24*x12 +-12*x13 +190*x14 +165*x15 +-97*x16 +43*x17 +-139*x18 +24*x19 +-73*x20 +-2474");
	problem.addConstraint("0<=-45*x1 +31*x2 +-8*x3 +-35*x4 +-105*x5 +55*x6 +-151*x7 +98*x8 +80*x9 +-76*x10 +-192*x11 +-296*x12 +93*x13 +38*x14 +-40*x15 +16*x16 +8*x17 +137*x18 +20*x19 +3*x20 +3154");
	problem.addConstraint("0<=61*x1 +-44*x2 +241*x3 +-63*x4 +109*x5 +88*x6 +-250*x7 +-5*x8 +-41*x9 +112*x10 +100*x11 +116*x12 +-27*x13 +19*x14 +-33*x15 +19*x16 +169*x17 +-78*x18 +14*x19 +30*x20 +-159");
	problem.addConstraint("0<=65*x1 +-65*x2 +-64*x3 +-80*x4 +82*x5 +-171*x6 +19*x7 +29*x8 +-121*x9 +-41*x10 +40*x11 +-145*x12 +-64*x13 +90*x14 +54*x15+-67*x16 +19*x17 +291*x18 +-134*x19 +100*x20 +2244");
	problem.addConstraint("0<=-98*x1 +66*x2 +-144*x3 +46*x4 +-99*x5 +150*x6 +-176*x7 +63*x8 +13*x9 +-86*x10 +96*x11 +-197*x12 +3*x13 +30*x14 +-77*x15 +-119*x16 +126*x17 +-2*x18 +-2*x19 +-199*x20 +5010");
	problem.addConstraint("0<=-88*x1 +-37*x2 +-52*x3 +43*x4 +-97*x5 +38*x6 +16*x7 +-17*x8 +165*x9 +140*x10 +13*x11 +-2*x12 +46*x13 +-38*x14 +121*x15 +124*x16 +266*x17 +168*x18 +-145*x19 +187*x20 +-6230");
	problem.addConstraint("0<=122*x1 +142*x2 +146*x3 +-31*x4 +169*x5 +-44*x6 +-83*x7 +-5*x8 +6*x9 +135*x10 +-54*x11 +-65*x12 +23*x13 +191*x14 +-12*x15+41*x16 +114*x17 +-26*x18 +-57*x19 +77*x20 +-4552");
	problem.addConstraint("0<=133*x1 +-178*x2 +119*x3 +-58*x4 +-11*x5 +-13*x6 +179*x7 +-88*x8 +23*x9 +-69*x10 +18*x11 +35*x12 +81*x13 +-175*x14 +217*x15 +-94*x16 +75*x17 +3*x18 +-228*x19 +114*x20 +267");
	problem.addConstraint("0<=193*x1 +153*x2 +15*x3 +-77*x4 +-50*x5 +-155*x6 +-64*x7 +-152*x8 +-58*x9 +-103*x10 +135*x11 +154*x12 +-193*x13 +25*x14 +-42*x15 +-34*x16 +-72*x17 +66*x18 +83*x19 +16*x20 +2842");
	problem.addConstraint("0<=-145*x1 +-129*x2 +-8*x3 +-7*x4 +26*x5 +-115*x6 +72*x7 +-39*x8 +-83*x9 +49*x10 +6*x11 +-8*x12 +63*x13 +-10*x14 +14*x15 +28*x16 +24*x17 +170*x18 +-33*x19 +146*x20 +20");
	problem.addConstraint("0<=83*x1 +-19*x2 +-119*x3 +20*x4 +55*x5 +204*x6 +-35*x7 +-33*x8 +-4*x9 +212*x10 +32*x11 +98*x12 +10*x13 +-182*x14 +58*x15 +82*x16 +-95*x17 +-103*x18 +288*x19 +37*x20 +-4458");
	problem.addConstraint("0<=-125*x1 +31*x2 +-56*x3 +-94*x4 +-25*x5 +-35*x6 +74*x7 +74*x8 +-33*x9 +-47*x10 +-128*x11 +-79*x12 +101*x13 +-142*x14 +163*x15 +-221*x16 +144*x17 +9*x18 +65*x19 +133*x20 +-289");
	problem.addConstraint("0<=-15*x1 +31*x2 +100*x3 +183*x4 +67*x5 +73*x6 +-129*x7 +-76*x8 +57*x9 +99*x10 +120*x11 +-209*x12 +-121*x13 +77*x14 +-52*x15 +-154*x16 +18*x17 +1*x18 +54*x19 +147*x20 +-4198");
	problem.addConstraint("0<=169*x1 +58*x2 +-164*x3 +232*x4 +-15*x5 +-19*x6 +-141*x7 +-44*x8 +111*x9 +92*x10 +-15*x11 +26*x12 +7*x13 +-37*x14 +-26*x15 +30*x16 +-5*x17 +-14*x18 +-212*x19 +9*x20 +-1130");
	problem.addConstraint("0<=75*x1 +-16*x2 +19*x3 +-127*x4 +148*x5 +107*x6 +-48*x7 +-12*x8 +11*x9 +9*x10 +-3*x11 +269*x12 +-78*x13 +32*x14 +82*x15 +-260*x16 +126*x17 +80*x18 +124*x19 +-54*x20 +-5457");
	problem.addConstraint("0<=-8*x1 +-11*x2 +-57*x3 +252*x4 +-118*x5 +0*x6 +-32*x7 +-83*x8 +-133*x9 +129*x10 +111*x11 +25*x12 +-133*x13 +-123*x14 +7*x15 +-34*x16 +93*x17 +6*x18 +-78*x19 +-85*x20 +1905");
	problem.addConstraint("0<=60*x1 +35*x2 +-19*x3 +33*x4 +52*x5 +-7*x6 +270*x7 +239*x8 +198*x9 +-56*x10 +22*x11 +-24*x12 +70*x13 +-82*x14 +82*x15 +-106*x16 +28*x17 +139*x18 +0*x19 +77*x20 +-9743");
	problem.addConstraint("0<=111*x1 +-25*x2 +50*x3 +42*x4 +-123*x5 +0*x6 +101*x7 +50*x8 +-14*x9 +-187*x10 +18*x11 +-137*x12 +-90*x13 +-80*x14 +-107*x15 +9*x16 +123*x17 +-3*x18 +-272*x19 +-141*x20 +6119");
	problem.addConstraint("0<=0*x1 +9*x2 +-3*x3 +-101*x4 +123*x5 +135*x6 +176*x7 +-18*x8 +105*x9 +-146*x10 +34*x11 +10*x12 +60*x13 +139*x14 +151*x15 +-89*x16 +103*x17 +117*x18 +57*x19 +-18*x20 +-8576");
	problem.addConstraint("0<=80*x1 +140*x2 +-79*x3 +113*x4 +-9*x5 +-109*x6 +-118*x7 +-149*x8 +84*x9 +44*x10 +112*x11 +-306*x12 +-241*x13 +-17*x14 +-45*x15 +-61*x16 +93*x17 +148*x18 +-123*x19 +-110*x20 +2765");
	problem.addConstraint("0<=18*x1 +194*x2 +43*x3 +19*x4 +-123*x5 +6*x6 +-49*x7 +-27*x8 +32*x9 +52*x10 +180*x11 +171*x12 +-44*x13 +-37*x14 +-120*x15 +89*x16 +-27*x17 +37*x18 +53*x19 +-30*x20 +-202");
	problem.addConstraint("0<=-230*x1 +-111*x2 +-63*x3 +30*x4 +-91*x5 +-14*x6 +-209*x7 +63*x8 +-73*x9 +-29*x10 +36*x11 +-33*x12 +-90*x13 +-71*x14 +163*x15 +-81*x16 +61*x17 +135*x18 +-55*x19 +-31*x20 +5390");
	problem.addConstraint("0<=184*x1 +70*x2 +42*x3 +84*x4 +-148*x5 +103*x6 +42*x7 +18*x8 +-16*x9 +88*x10 +129*x11 +84*x12 +-87*x13 +-80*x14 +-72*x15 +196*x16 +46*x17 +-109*x18 +77*x19 +92*x20 +-2489");
	problem.addConstraint("0<=7*x1 +-4*x2 +8*x3 +76*x4 +6*x5 +56*x6 +-198*x7 +-20*x8 +40*x9 +-86*x10 +38*x11 +-145*x12 +-90*x13 +25*x14 +145*x15 +-97*x16 +-72*x17 +-10*x18 +70*x19 +-97*x20 +1452");
	problem.addConstraint("0<=110*x1 +45*x2 +4*x3 +-36*x4 +-1*x5 +-22*x6 +87*x7 +-260*x8 +-23*x9 +9*x10 +-15*x11 +-76*x12 +93*x13 +23*x14 +5*x15 +29*x16 +-31*x17 +48*x18 +60*x19 +103*x20 +-1248");
	problem.addConstraint("0<=-51*x1 +191*x2 +-57*x3 +108*x4 +-42*x5 +-8*x6 +-78*x7 +56*x8 +72*x9 +-255*x10 +-26*x11 +86*x12 +-195*x13 +197*x14 +164*x15 +-15*x16 +135*x17 +-136*x18 +98*x19 +-87*x20 +-3192");
	problem.addConstraint("0<=-177*x1 +139*x2 +119*x3 +-32*x4 +-73*x5 +10*x6 +-82*x7 +116*x8 +-86*x9 +81*x10 +80*x11 +-210*x12 +-158*x13 +126*x14 +95*x15 +13*x16 +-170*x17 +69*x18 +4*x19 +51*x20 +2344");

	auto obj = problem.addObjective("23*x1 +2432*x2 +-1668*x3 +1362*x4 +570*x5 +1175*x6 +-3120*x7 +-1763*x8 +867*x9 +637*x10 +4600*x11 +2048*x12 +-1366*x13 +-309*x14 +229*x15 +-2918*x16 +1893*x17 +2686*x18 +-1377*x19 +-1775*x20");

	SolutionStats res = problem.solve();

	std::cout << *(problem.tab) << '\n';

	assert(x1->value == 5);
	assert(x2->value == 6);
	assert(x3->value == 2);
	assert(x4->value == 13);
	assert(x5->value == 12);
	assert(x6->value == 7);
	assert(x7->value == 12);
	assert(x8->value == 5);
	assert(x9->value == 13);
	assert(x10->value == 7);
	assert(x11->value == 2);
	assert(x12->value == 6);
	assert(x13->value == 4);
	assert(x14->value == 10);
	assert(x15->value == 9);
	assert(x16->value == 0);
	assert(x17->value == 9);
	assert(x18->value == 5);
	assert(x19->value == 10);
	assert(x20->value == 8);

	assert(obj->value == 31109);
}

int main(void) {
	test_random_10x5();
	test_random_40x20();

	return 0;
}
