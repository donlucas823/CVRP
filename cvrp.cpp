//#include <iostream>
//#include <vector>
//#include <cmath>
//#include <algorithm>
//#include <iomanip>
//#include "gurobi_c++.h"
//#include <string>
//using namespace std;
//
//#define VehicleNum 2
//#define CustomNum 10
//#define VehicleCapacity 100
//
//vector<int>Demand(CustomNum);
//vector<vector<double>>DistanceMatrix(CustomNum, vector<double>(CustomNum));
//
//GRBVar x[VehicleNum][CustomNum][CustomNum];
//GRBVar y[VehicleNum][CustomNum];
//GRBVar u[VehicleNum][CustomNum];
//string varName = { 0 };
//
//const double CustomCoordinate[][2] = {
//{ 23, 95},{820,915},{470,302},{950,905},{868,744},{891,987},
//{ 76,625},{  8,541},{219,988},{662,636},{318,402},{307,984}
//};
//
////设置随机数
//class SetNumber {
//public:
//	SetNumber() { srand((unsigned)time(NULL)); }
//	int operator()(int begin, int end) { //set Demand
//		return (begin + rand() % (end - begin + 1));
//	}
//	double operator()(const double* city1, const double* city2) { //set Distance
//		return sqrt(pow((city1[0] - city2[0]), 2) + pow((city1[1] - city2[1]), 2));
//	}
//}setNum;
//
////生成距离矩阵和需求矩阵
//void setDistanceAndDemand() {
//	for (size_t i = 0; i < CustomNum; i++) {
//		Demand[i] = setNum(10, 20);
//		//cout << Demand[i] << " ";
//		for (size_t j = 0; j < CustomNum; j++) {
//			if (i == j)DistanceMatrix[i][j] = 0.0;
//			else DistanceMatrix[i][j] = setNum(CustomCoordinate[i], CustomCoordinate[j]);
//			//cout << setw(7) <<setiosflags(ios::fixed) << setprecision(2) 
//			//<< DistanceMatrix[i][j] << " ";
//		}//cout << endl;	
//	}//cout << endl;
//}
//
////打印需求
//class myPrint {
//public:
//	myPrint() { cout << endl << "Demand: "; }
//	void operator()(int demand) {
//		cout << demand << " ";
//	}
//	~myPrint() { cout << endl; }
//};
////生成变量名
//string getVarName(string head, string& varName, int k, int i, int j = -1)
//{
//	varName.erase();
//	varName.append(head);
//	varName.append("_");
//	varName.append(to_string(k));
//	varName.append("_");
//	varName.append(to_string(i));
//	if (j != -1)
//	{
//		varName.append("_");
//		varName.append(to_string(j));
//	}
//	return varName;
//}
//
//
//int main() {
//	setDistanceAndDemand();
//
//	try {
//		GRBEnv env = GRBEnv();
//		env.start();
//		GRBModel model = GRBModel(env);
//		//决策变量 
//		for (size_t k = 0; k < VehicleNum; k++) {
//			for (size_t i = 0; i < CustomNum; i++) {
//				for (size_t j = 0; j < CustomNum; j++) {
//					if (i != j) {
//						getVarName("x", varName, k, i, j);
//						x[k][i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, varName);
//					}
//				}
//			}
//		}
//		for (size_t k = 0; k < VehicleNum; k++) {
//			for (size_t i = 0; i < CustomNum; i++) {
//				getVarName("y", varName, k, i);
//				y[k][i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, varName);
//			}
//		}
//		for (size_t k = 0; k < VehicleNum; k++) {
//			for (size_t i = 0; i < CustomNum; i++) {
//				getVarName("u", varName, k, i);
//				u[k][i] = model.addVar(0.0, VehicleCapacity, 0.0, GRB_INTEGER, varName);
//			}
//		}
//
//		//目标函数 路径最短
//		GRBLinExpr obj = 0;
//		for (size_t k = 0; k < VehicleNum; k++) {
//			for (size_t i = 0; i < CustomNum; i++) {
//				for (size_t j = 0; j < CustomNum; j++) {
//					if (i != j)
//						obj += x[k][i][j] * DistanceMatrix[i][j];
//				}
//			}
//		}
//		model.setObjective(obj, GRB_MINIMIZE);
//
//		//约束1 顾客被访问一次
//		GRBLinExpr constr1 = 0.0;
//		for (size_t i = 0; i < CustomNum; i++) {
//			for (size_t k = 0; k < VehicleNum; k++) {
//				constr1 += y[k][i];
//			}
//			model.addConstr(constr1, GRB_EQUAL, 1);
//			constr1 = 0.0;
//		}
//
//		//约束2 流平衡	
//		GRBLinExpr constr2 = 0;
//		for (size_t i = 0; i < CustomNum; i++) {
//			for (size_t k = 0; k < VehicleNum; k++) {
//				for (size_t j = 0; j < CustomNum; j++) {
//					if (i != j) {
//						constr2 += x[k][i][j];
//					}
//				}
//				model.addConstr(constr2, GRB_EQUAL, y[k][i]);
//				constr2 = 0.0;
//			}
//		}
//		//约束3 流平衡			
//		GRBLinExpr constr3 = 0;
//		for (size_t j = 0; j < CustomNum; j++) {
//			for (size_t k = 0; k < VehicleNum; k++) {
//				for (size_t i = 0; i < CustomNum; i++) {
//					if (i != j) {
//						constr3 += x[k][i][j];
//					}
//				}
//				model.addConstr(constr3, GRB_EQUAL, y[k][j]);
//				constr3 = 0.0;
//			}
//		}
//		//约束4 载重约束
//		GRBLinExpr constr4 = 0;
//		for (size_t k = 0; k < VehicleNum; k++) {
//			for (size_t i = 0; i < CustomNum; i++) {
//				constr4 += y[k][i] * Demand[i];
//			}
//			model.addConstr(constr4, GRB_LESS_EQUAL, VehicleCapacity);
//			constr4 = 0.0;
//		}
//		//约束5 MTZ约束避免子回路
//		for (size_t i = 0; i < CustomNum - 1; i++) {
//			for (size_t j = 1; j < CustomNum; j++) {
//				if (i != j) {
//					for (size_t k = 0; k < VehicleNum; k++) {
//						GRBLinExpr constr5_1 = u[k][j] - u[k][i] + x[k][i][j] * VehicleCapacity;
//						GRBLinExpr constr5_2 = VehicleCapacity - Demand[i];
//						model.addConstr(constr5_1, GRB_LESS_EQUAL, constr5_2);
//						constr5_1 = 0.0;
//						constr5_2 = 0.0;
//					}
//				}
//			}
//		}
//		//优化
//		model.optimize();
//
//		//打印随机生成的需求
//		for_each(Demand.begin(), Demand.end(), myPrint());
//
//		//输出优化结果
//		for (size_t k = 0; k < VehicleNum; k++) {
//			for (size_t i = 0; i < CustomNum; i++) {
//				for (size_t j = 0; j < CustomNum; j++) {
//					if (i != j && x[k][i][j].get(GRB_DoubleAttr_X) == 1)
//						cout << setw(10) << x[k][i][j].get(GRB_StringAttr_VarName) << " = "
//						<< x[k][i][j].get(GRB_DoubleAttr_X) << "; ";
//				}
//			}cout << endl;
//		}
//		for (size_t k = 0; k < VehicleNum; k++) {
//			for (size_t i = 0; i < CustomNum; i++) {
//				if (y[k][i].get(GRB_DoubleAttr_X) == 1)
//					cout << setw(10) << y[k][i].get(GRB_StringAttr_VarName) << " = "
//					<< y[k][i].get(GRB_DoubleAttr_X) << "; ";
//			}cout << endl;
//		}
//	}
//
//	catch (GRBException e) {
//		cout << "Error code: " << e.getErrorCode() << endl;
//		cout << e.getMessage();
//	}
//
//	catch (...) {
//		cout << "Exception during optimization" << endl;
//	}
//	return EXIT_SUCCESS;
//}