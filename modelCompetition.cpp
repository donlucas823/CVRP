//#include <iostream>
//#include "gurobi_c++.h"
//using namespace std;
//
//#define M 3000//������Ʒ����
//#define N 125//����ר�ҵ�����
//#define K 5//ÿ����Ʒ��5λר������
//#define C 120//ƽ��ÿλר��������Ʒ����
//#define U 126//ÿλר��������Ʒ��������(C�ϸ�5%,126����Ʒ)
//#define L 114//ÿλר��������Ʒ��������(C�¸�5%,114����Ʒ)
//
//GRBVar x[M][N];
//
//int main() {
//	try {
//		GRBEnv env = GRBEnv();
//		env.start();
//		GRBModel model = GRBModel(env);
//
//		for (size_t i = 0; i < M; i++) 		//���߱��� 
//			for (size_t j = 0; j < N; j++) 
//				x[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY);
//			
//		for (size_t i = 0; i < M; i++) {	//Լ��1 ÿ����Ʒ�����뱻Kλר������
//			GRBLinExpr constr1 = 0.0;
//			for (size_t j = 0; j < N; j++)
//				constr1 += x[i][j];
//			model.addConstr(constr1, GRB_EQUAL, K);
//			constr1 = 0.0;
//		}
//
//		for (size_t j = 0; j < N; j++) {	//Լ��2 ÿλר��������Ʒ��������Լ��
//			GRBLinExpr constr2 = 0.0;
//			for (size_t i = 0; i < M; i++)
//				constr2 += x[i][j];
//			model.addConstr(constr2, GRB_LESS_EQUAL, U);
//			constr2 = 0.0;
//		}
//
//		for (size_t j = 0; j < N; j++) {	//Լ��3 ÿλר��������Ʒ��������Լ��
//			GRBLinExpr constr3 = 0.0;
//			for (size_t i = 0; i < M; i++)
//				constr3 += x[i][j];
//			model.addConstr(constr3, GRB_GREATER_EQUAL, L);
//			constr3 = 0.0;
//		}
//
//		GRBQuadExpr obj = 0;				//Ŀ�꺯�� �ɱ���ָ�����
//		for (size_t j = 0; j < N; j++)
//			for (size_t j0 = j; j0 < N; j0++)
//				for (size_t i = 0; i < M; i++)
//					obj += x[i][j] * x[i][j0];
//		model.setObjective(obj, GRB_MAXIMIZE);
//		
//		model.optimize();					//�Ż�
//
//		for (size_t i = 0; i < M; i++) {	//����Ż����
//			for (size_t j = 0; j < N; j++) {
//				cout << x[i][j].get(GRB_DoubleAttr_X) << "; ";
//			}
//			cout << endl;
//		}
//		cout << "���Ŀɱ���ָ��Ϊ" << model.get(GRB_DoubleAttr_ObjVal) << endl;
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