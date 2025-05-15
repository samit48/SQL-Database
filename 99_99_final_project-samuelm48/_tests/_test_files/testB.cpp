#include "gtest/gtest.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>

#include "../../includes/sql/sql.h"

using namespace std;

const vector<string> command_list = {

/*00*/     "make table employee fields  last,       first,         dep,      salary, year",
/*01*/     "insert into employee values Blow,       Joe,           CS,       100000, 2018",
/*02*/     "insert into employee values Blow,       JoAnn,         Physics,  200000, 2016",
/*03*/     "insert into employee values Johnson,    Jack,          HR,       150000, 2014",
/*04*/     "insert into employee values Johnson,    \"Jimmy\",     Chemistry,140000, 2018",

/*05*/     "make table student fields  fname,          lname,    major,    age",
/*06*/     "insert into student values Flo,            Yao, 	Art, 	20",
/*07*/     "insert into student values Bo, 		     Yang, 	CS, 		28",
/*08*/     "insert into student values \"Sammuel L.\", Jackson, 	CS, 		40",
/*09*/     "insert into student values \"Billy\",	     Jackson, 	Math,	27",
/*10*/     "insert into student values \"Mary Ann\",   Davis,	Math,	30",
/*22*/     "insert into student values Yao, \"Mary Ann\", Chemistry, 28", 

"select * from employee where last >= Jack",
/*11*/     "select * from employee",
/*12*/     "select last, first, age from employee",
/*13*/     "select last from employee",
/*14*/     "select * from employee where last = Johnson",
/*15*/     "select * from employee where last=Blow and major=\"JoAnn\"",

/*16*/     "select * from student",
/*17*/     "select * from student where (major=CS or major=Art)",
/*18*/     "select * from student where (lname>J)",
/*19*/     "select * from student where ((lname>J) and (major=CS or major=Art))",

/*20*/     "insert into student values Jones,\"Sue Ellen\", CS, 20",
/*20.1*/   "select * from student where lname = \"Yao\" and fname = \"Flo\"",
/*21*/     "select lname, fname from student where lname = \"Yao\" and fname = \"Flo\"",
/*22*/     "select lname, fname from student where lname = \"Mary Ann\" and fname = Yao",
/*23*/     "select * from student where (age>=25 or lname = Yao) and (fname = \"Teresa Mae\" or Major = CS)",
/*24*/     "select * from student where lname = Jackson",
/*25*/     "select * from student where lname = \"Del Rio\"",
/*26*/     "select * from student where lname = \"Does Not Exist\"",
/*27*/     "select * from student where lname > Yang",
/*28*/     "select * from student where age > 50",
/*29*/     "select * from student where age  < 53",
/*30*/     "select * from student where age > 54",
/*28*/     "select * from student where age > 22",
/*29*/     "select * from student where age  > 34",
/*30*/     "select * from student where age > 28",
/*32*/     "select * from student where lname >= Yang",
/*33*/     "select * from employee where last >= Jack",
/*34*/     "select * from student where lname < Jackson",
/*35*/     "select * from student where age  < 30",
/*36*/     "select * from student where age  < 26",
/*37*/     "select * from student where lname <= Smith",
/*39*/     "select * from employee where last <= Peach",
/*40*/     "select * from student where fname = \"Flo\" and lname = \"Yao\"",
/*41*/     "select * from student where fname = Flo or lname = Jackson",


/*42*/     "select * from student where fname = Flo or major = CS and age <= 23",


"select * from student where fname = Flo and major = CS or age <= 23",


/*43*/     "select * from student where age <30 and major=CS or major = Physics and lname = Jackson",

/*44*/     "select * from student where lname = Yang or major = CS and age < 23 or lname = Jackson"

};

const int MAKE_TABLE_COMMANDS = 11;
const int SELECT_COMMANDS = 53;

bool test_stub(bool debug = false)
{
  SQL sql;
  Table t;
  cout << ">" << command_list[0] << endl;
  sql.command(command_list[0]);
  cout << "basic_test: table created." << endl
       << endl;

  for (int i = 0; i < MAKE_TABLE_COMMANDS; i++)
  {
    cout << ">" << command_list[i] << endl;
    sql.command(command_list[i]);
  }

  cout << endl
       << endl;
  for (int i = MAKE_TABLE_COMMANDS; i < command_list.size(); i++)
  {
    cout << "\n>" << command_list[i] << endl;
    if (debug)
      cout << sql.command(command_list[i]) << endl;
    else
      t = sql.command(command_list[i]);
    cout << "basic_test: records selected: " << sql.select_recnos() << endl;
  }

  return true;
}

TEST(TEST_STUB, TestStub)
{

  // EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_stub(false));
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  std::cout << "\n\n----------running testB.cpp---------\n\n"
            << std::endl;
  return RUN_ALL_TESTS();
}
