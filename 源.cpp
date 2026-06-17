#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
using namespace std;

//学生类
class Student
{
public:
    string id;//学号
    string name;//姓名
    map<string, double> scores;//map，string是课程名，double是分数

    //默认构造函数，给后面vector用
    Student() {}
    //带参构造函数
    Student(string sid, string sname) : id(sid), name(sname) {}

    //计算单个学生总分函数
    double getTotal()
    {
        double total = 0.0;
        for (auto& p : scores)
            total += p.second;
        return total;
    }

    //计算单个学生平均分函数
    double getAvg()
    {
        if (scores.empty())
            return 0.0;//防止除数为0
        return getTotal() / scores.size();
    }
};

//成绩管理类
class Manager
{
public:
    vector<Student>stuList;  //vector存储学生信息
    string fileName;//此变量为导出的数据文件名称

    //冒泡排序
    void sort()
    {
        int total = stuList.size();
        for (int i = 0; i < total - 1; i++)
        {
            for (int j = 0; j < total-1- i; j++)
            {
                //以平均分作为排序依据
                if (stuList[j].getAvg() < stuList[j + 1].getAvg())
                {
                    Student t = stuList[j];
                    stuList[j] = stuList[j + 1];
                    stuList[j + 1] = t;
                }
            }
        }
    }


    //构造函数：初始化文件名，加载文件数据
    Manager(string file) : fileName(file)
    {
        loadData();
    }

    //功能：添加学生及成绩信息
    void newstu()
    {
        string id, name, course;
        double score;
        cout << "===== 添加学生及成绩信息 =====" << endl;
        cout << "请输入学号：";
        cin >> id;
        cout << "请输入姓名：";
        cin >> name;
        cout << "请输入课程名称：";
        cin >> course;
        cout << "请输入成绩(成绩范围：0-100)：";

        //输入容错机制
        while (!(cin >> score) || score < 0 || score > 100)
        {
            cin.clear();//清理错误标记
            cin.ignore(1024, '\n');//忽略缓冲区最多1024字符，直到碰见回车键，防止卡住
            cout << "输入有误，成绩范围应是0-150。请重新输入：";
        }

        bool a = 0;
        for (auto& stu : stuList)//auto自动匹配Student，stu，其在遍历容器的过程中每次取容器中其中一个学生的信息
        {
            if (stu.id == id)
            {
                a = 1;
                stu.scores[course] = score;
                cout << "该学生存在，已对现有成绩更新完毕" << endl;//自动更新成绩
                break;
            }
        }
        if (a==0)//不存在的学生，那就是新增喽
        {
            Student newStu(id, name);
            newStu.scores[course] = score;
            stuList.push_back(newStu);
            cout << "新学生学生信息添加完毕" << endl;
        }
    }

    //功能：删除指定学生信息
    void deletestu()
    {
        string id;
        cout << "===== 删除学生信息 =====" << endl;
        cout << "请输入该生学号：";
        cin >> id;

        for (auto it = stuList.begin(); it != stuList.end(); it++)//根据学号检索
        {
            if (it->id == id)
            {
                stuList.erase(it);
                cout << "该生已删除" << endl;
                return;
            }
        }
        cout << "未查询到学生" << endl;
    }

    //功能：修改学生课程成绩
    void change()
    {
        string id, course;
        double newScore;
        cout << "===== 修改学生成绩 =====" << endl;
        cout << "请输入该生学号：";
        cin >> id;
        cout << "请输入要修改成绩的课程名：";
        cin >> course;

        for (auto& stu : stuList)
        {
            if (stu.id == id)
            {
                if (stu.scores.find(course) == stu.scores.end())//前面map容器形式的scores的自带函数find
                {
                    cout << "该学生无此课程成绩！" << endl;
                    return;
                }
                cout << "请输入新成绩(成绩范围：0-100)：";
                while (!(cin >> newScore) || newScore < 0 || newScore > 100)
                {
                    cin.clear();
                    cin.ignore(1024, '\n');
                    cout << "输入无效！请重新输入：";//依旧防止意外发生，同上
                }
                stu.scores[course] = newScore;
                cout << "成绩修改成功！" << endl;
                return;
            }
        }
        cout << "未查询到该学号学生！" << endl;
    }

    //功能：查询单个学生成绩
    void searchstu()
    {
        string id;
        cout << "===== 查询学生成绩 =====" << endl;
        cout << "请输入学号：";
        cin >> id;

        for (auto& stu : stuList)
        {
            if (stu.id == id)
            {
                cout << "---查询成功---" << endl;
                cout<< "学号：" << stu.id << "姓名：" << stu.name << endl;
                cout<< "课程成绩：" << endl;
                for (auto& p : stu.scores)
                {
                    cout << p.first << "：" << p.second << "；"<< endl;//first对应的是科目，second对应的是成绩
                }
                cout << "总分：" << stu.getTotal() << "平均分：" << stu.getAvg() << endl;
                return;
            }
        }
        cout << "未查询到该学号学生！" << endl;
    }

    //功能：展示全部学生成绩
    void showall()
    {
        if (stuList.empty())
        {
            cout << "暂无学生数据，请检查是否已录入学生" << endl;
            return;//异常处理，防止空集
        }
        cout << "\n==================== 全体学生成绩 ====================" << endl;
        for (auto& stu : stuList)
        {
            cout << "学号：" << stu.id << " 姓名：" << stu.name;
            cout << "总分：" << stu.getTotal() << " 平均分：" << stu.getAvg() << endl;
            cout << " 各课程成绩：";
            for (const auto& p : stu.scores)
                cout << p.first << "：" << p.second << "；";
            cout << endl;
        }
    }

    //功能：统计制定课程所有学生的最高分、最低分、平均分
    void countscore()
    {
        if (stuList.empty())
        {
            cout << "暂无学生数据，请检查是否已录入学生" << endl;
            return;
        }

        string classes;
        cout << "请输入制定课程名称：";
        cin >> classes;

        double numscores = 0.0;
        double max = 0.0, min = 100.0;
        int studentnum = 0;

        // 遍历所有学生
        for (auto& stu : stuList)
        {
            // 查找该学生是否具有该课程成绩
            auto p = stu.scores.find(classes);
            if (p == stu.scores.end())
                continue; // 无该课程则跳过

            double tempscore = p->second;
            numscores =numscores+tempscore;
            studentnum++;

            if (tempscore > max)
            {
                max = tempscore;
            }
                
            if (tempscore < min)
            {
                min = tempscore;
            }
        }

        // 判断是否存在该课程成绩
        if (studentnum == 0)
        {
            cout << "暂无学生修读此课程" << endl;
            return;
        }

        double avg = numscores / studentnum;
        cout << "统计结果" << endl;
        cout << "选修该课程总人数：" << studentnum << endl;
        cout << "课程最高分：" << max<< endl;
        cout << "课程最低分：" << min<< endl;
        cout << "课程平均分：" << avg << endl;
    }

    //功能：平均分降序排名
    void sortavg()
    {
        if (stuList.empty())
        {
            cout << "暂无数据，请检查是否录入数据" << endl;
            return;
        }
        sort();
        cout << "已按平均分从高到低完成排名！" << endl;
        showall();
    }

    //功能：挂科学生鞭尸环节
    void showfail()
    {
        bool a = 0;
        cout << "\n===== 不及格学生名单 =====" << endl;
        for (auto& stu :stuList)
        {
            for (auto& p :stu.scores)
            {
                if (p.second < 60)
                {
                    a = 1;
                    cout << "学号：" << stu.id << "姓名：" << stu.name;
                    cout << " 课程：" << p.first << "成绩：" << p.second << endl;
                    break;
                }
            }
        }
        if (a==0)
            cout << "暂无不及格学生！" << endl;
    }

    //功能：保存数据到文件（纯vibe coding）
    void savedata()
    {
        ofstream out(fileName, ios::out);
        if (!out)
        {
            cout << "文件打开异常" << endl;
            return;
        }
        for (auto& stu : stuList)
        {
            out << stu.id << " " << stu.name << " " << stu.scores.size() << " ";
            for (auto& p : stu.scores)
                out << p.first << " " << p.second << " ";
            out << endl;
        }
        out.close();
        cout << "已保存数据到文件" << endl;
    }

    //功能：从文件加载数据（纯vibe coding）
    void loadData()
    {
        ifstream in(fileName, ios::in);
        if (!in)
        {
            cout << "数据文件不存在，系统将新建文件" << endl;
            return;
        }
        stuList.clear();
        string id, name, course;
        int courseNum;
        double score;
        while (in >> id >> name >> courseNum)//从文件中连续读取学号、姓名、课程门数，读取失败或没有时自动终止
        {
            Student stu(id, name);
            for (int i = 0; i < courseNum; i++)
            {
                in >> course >> score;
                stu.scores[course] = score;
            }
            stuList.push_back(stu);//把此时的临时stu对象（刚读取的）添加到vector末尾，即新增成员
        }
        in.close();
        cout << "文件数据读取完成，当前共" << stuList.size() << "名学生" << endl;
    }
};

//主菜单函数
void menu()
{
    cout << "\n********** 学生成绩管理系统 **********" << endl;
    cout << "请输入对应数字并回车进入对应功能......" << endl;
    cout << "1. 添加学生成绩信息" << endl;
    cout << "2. 删除学生信息" << endl;
    cout << "3. 修改学生成绩" << endl;
    cout << "4. 查询学生成绩" << endl;
    cout << "5. 显示所有学生成绩" << endl;
    cout << "6. 成绩统计(平均分/最高分/最低分)" << endl;
    cout << "7. 按平均分排名" << endl;
    cout << "8. 查看不及格学生名单" << endl;
    cout << "9. 手动保存数据到文件" << endl;
    cout << "0. 退出系统" << endl;
    cout << "########################################" << endl;
    cout << "请输入对应功能前数字：";
}

int main()
{
    Manager manager("数据库.txt");
    int choice;

    while (1)
    {
        menu();
        while (!(cin >> choice))
        {
            cin.clear();//还原cin
            cin.ignore(1024, '\n');//清空缓冲区所有残留字符直到换行，避免死循环
            cout << "输入无效，请重新输入0-9间的阿拉伯数字编号：";//输入异常处理，同上
        }

        switch (choice)
        {
        case 1: manager.newstu(); break;
        case 2: manager.deletestu(); break;
        case 3: manager.change(); break;
        case 4: manager.searchstu(); break;
        case 5: manager.showall(); break;
        case 6: manager.countscore(); break;
        case 7: manager.sortavg(); break;
        case 8: manager.showfail(); break;
        case 9: manager.savedata(); break;
        case 0:
            cout << "退出系统，数据已自动保存！" << endl;
            manager.savedata();
            return 0;
        default:
            cout << "编号错误，请重新选择" << endl;
        }
        cout << "\n按下回车以继续......";
        cin.ignore(1024, '\n');//清空防止之前输入的回车被读取导致用户一脸懵
        cin.get();
    }
    return 0;
}