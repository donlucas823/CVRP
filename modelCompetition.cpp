//#include <iostream>
//#include "gurobi_c++.h"
//using namespace std;
//
//#define M 3000//参赛作品数量
//#define N 125//评审专家的数量
//#define K 5//每份作品由5位专家评审
//#define C 120//平均每位专家评审作品数量
//#define U 126//每位专家评审作品数量上限(C上浮5%,126份作品)
//#define L 114//每位专家评审作品数量下限(C下浮5%,114份作品)
//
//GRBVar x[M][N];
//
//int main() {
//	try {
//		GRBEnv env = GRBEnv();
//		env.start();
//		GRBModel model = GRBModel(env);
//
//		for (size_t i = 0; i < M; i++) 		//决策变量 
//			for (size_t j = 0; j < N; j++) 
//				x[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY);
//			
//		for (size_t i = 0; i < M; i++) {	//约束1 每份作品都必须被K位专家评审
//			GRBLinExpr constr1 = 0.0;
//			for (size_t j = 0; j < N; j++)
//				constr1 += x[i][j];
//			model.addConstr(constr1, GRB_EQUAL, K);
//			constr1 = 0.0;
//		}
//
//		for (size_t j = 0; j < N; j++) {	//约束2 每位专家评审作品数量上限约束
//			GRBLinExpr constr2 = 0.0;
//			for (size_t i = 0; i < M; i++)
//				constr2 += x[i][j];
//			model.addConstr(constr2, GRB_LESS_EQUAL, U);
//			constr2 = 0.0;
//		}
//
//		for (size_t j = 0; j < N; j++) {	//约束3 每位专家评审作品数量下限约束
//			GRBLinExpr constr3 = 0.0;
//			for (size_t i = 0; i < M; i++)
//				constr3 += x[i][j];
//			model.addConstr(constr3, GRB_GREATER_EQUAL, L);
//			constr3 = 0.0;
//		}
//
//		GRBQuadExpr obj = 0;				//目标函数 可比性指标最大
//		for (size_t j = 0; j < N; j++)
//			for (size_t j0 = j; j0 < N; j0++)
//				for (size_t i = 0; i < M; i++)
//					obj += x[i][j] * x[i][j0];
//		model.setObjective(obj, GRB_MAXIMIZE);
//		
//		model.optimize();					//优化
//
//		for (size_t i = 0; i < M; i++) {	//输出优化结果
//			for (size_t j = 0; j < N; j++) {
//				cout << x[i][j].get(GRB_DoubleAttr_X) << "; ";
//			}
//			cout << endl;
//		}
//		cout << "最大的可比性指标为" << model.get(GRB_DoubleAttr_ObjVal) << endl;
//	}
//	
//	catch (GRBException e) {
//		cout << "Error Code :" << e.getErrorCode() << endl;
//		cout << e.getMessage() << endl;
//	}
//	
//	catch (...) {
//		cout << "Exception during optimization!" << endl;
//	}
//}