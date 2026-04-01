#include "../src/RemoteControl.h"
#include "../src/TVSet.h"
#include <gtest/gtest.h>
#include <optional>
#include <sstream>
#include <string>

TEST(TVSetBasicTest, InitiallyTurnedOff)
{
	const TVSet tv;
	EXPECT_FALSE(tv.IsTurnedOn());
	EXPECT_EQ(tv.GetChannel(), 0);
}

TEST(TVSetBasicTest, TurnOnFirstTimeStartsAtChannelOne)
{
	TVSet tv;
	tv.TurnOn();
	EXPECT_TRUE(tv.IsTurnedOn());
	EXPECT_EQ(tv.GetChannel(), 1);
}

TEST(TVSetBasicTest, TurnOffRemembersLastChannelForNextTurnOn)
{
	TVSet tv;
	tv.TurnOn();
	tv.SelectChannelByNumber(2);
	tv.TurnOff();

	EXPECT_FALSE(tv.IsTurnedOn());
	EXPECT_EQ(tv.GetChannel(), 0);

	tv.TurnOn();
	EXPECT_TRUE(tv.IsTurnedOn());
	EXPECT_EQ(tv.GetChannel(), 2);
}

TEST(TVSetBasicTest, SelectChannelOnlyWhenOn)
{
	TVSet tv;
	EXPECT_TRUE(tv.SelectChannelByNumber(50));
	EXPECT_EQ(tv.GetChannel(), 0);
}

TEST(TVSetBasicTest, SelectValidChannelsRange)
{
	TVSet tv;
	tv.TurnOn();

	EXPECT_FALSE(tv.SelectChannelByNumber(1));
	EXPECT_EQ(tv.GetChannel(), 1);

	EXPECT_FALSE(tv.SelectChannelByNumber(99));
	EXPECT_EQ(tv.GetChannel(), 99);

	EXPECT_FALSE(tv.SelectChannelByNumber(TVSet::MIN_CHANNEL));
	EXPECT_FALSE(tv.SelectChannelByNumber(TVSet::MAX_CHANNEL));
}

TEST(TVSetBasicTest, SelectInvalidChannelFailsAndDoesNotChange)
{
	TVSet tv;
	tv.TurnOn();
	tv.SelectChannelByNumber(5);

	EXPECT_TRUE(tv.SelectChannelByNumber(0));
	EXPECT_TRUE(tv.SelectChannelByNumber(100));
	EXPECT_TRUE(tv.SelectChannelByNumber(-1));

	EXPECT_EQ(tv.GetChannel(), 5);
}

TEST(TVSetBasicTest, InfoStateWhenOff)
{
	const TVSet tv;
	EXPECT_FALSE(tv.IsTurnedOn());
	EXPECT_EQ(tv.GetChannel(), 0);
}

TEST(TVSetPrevChannelTest, FailsWhenTurnedOff)
{
	TVSet tv;
	tv.TurnOn();
	tv.SelectChannelByNumber(10);
	tv.TurnOff();

	EXPECT_TRUE(tv.SelectPreviousChannel());
}

TEST(TVSetPrevChannelTest, FailsIfChannelNeverChanged)
{
	TVSet tv;
	tv.TurnOn();

	EXPECT_TRUE(tv.SelectPreviousChannel());
	EXPECT_EQ(tv.GetChannel(), 1);
}

TEST(TVSetPrevChannelTest, SwitchesBackAndForthSimple)
{
	TVSet tv;
	tv.TurnOn();

	tv.SelectChannelByNumber(2);
	tv.SelectChannelByNumber(5);

	EXPECT_EQ(tv.GetChannel(), 5);

	EXPECT_FALSE(tv.SelectPreviousChannel());
	EXPECT_EQ(tv.GetChannel(), 2);

	EXPECT_FALSE(tv.SelectPreviousChannel());
	EXPECT_EQ(tv.GetChannel(), 5);
}

TEST(TVSetPrevChannelTest, PreservesHistoryAfterPowerCycle)
{
	TVSet tv;

	tv.TurnOn();
	tv.SelectChannelByNumber(10);
	tv.TurnOff();

	EXPECT_TRUE(tv.SelectPreviousChannel());

	tv.TurnOn();
	EXPECT_EQ(tv.GetChannel(), 10);

	EXPECT_FALSE(tv.SelectPreviousChannel());
	EXPECT_EQ(tv.GetChannel(), 1);
}

TEST(TVSetNamesTest, OperationsOnlyWhenOn)
{
	TVSet tv;
	EXPECT_TRUE(tv.SetChannelName(5, "ORT"));
	EXPECT_TRUE(tv.DeleteChannelName("MTV"));
	EXPECT_FALSE(tv.GetChannelName(5).has_value());
	EXPECT_FALSE(tv.GetChannelByName("ORT").has_value());
	EXPECT_TRUE(tv.SelectChannelByName(std::string("ORT")));
}

TEST(TVSetNamesTest, SetAndGetBidirectional)
{
	TVSet tv;
	tv.TurnOn();

	EXPECT_FALSE(tv.SetChannelName(5, "ORT"));

	const auto name = tv.GetChannelName(5);
	ASSERT_TRUE(name.has_value());
	EXPECT_EQ(*name, "ORT");

	const auto ch = tv.GetChannelByName("ORT");
	ASSERT_TRUE(ch.has_value());
	EXPECT_EQ(*ch, 5);
}

TEST(TVSetNamesTest, NormalizeSpaces)
{
	TVSet tv;
	tv.TurnOn();

	EXPECT_FALSE(tv.SetChannelName(1, "  Discovery   Channel  "));

	const auto name = tv.GetChannelName(1);
	ASSERT_TRUE(name.has_value());
	EXPECT_EQ(*name, "Discovery Channel");

	const auto ch = tv.GetChannelByName(" Discovery  Channel ");
	ASSERT_TRUE(ch.has_value());
	EXPECT_EQ(*ch, 1);
}

TEST(TVSetNamesTest, OneToOneMappingReassignment)
{
	TVSet tv;
	tv.TurnOn();

	tv.SetChannelName(7, "News");
	EXPECT_EQ(*tv.GetChannelByName("News"), 7);

	tv.SetChannelName(10, "News");

	EXPECT_FALSE(tv.GetChannelName(7).has_value());
	EXPECT_EQ(*tv.GetChannelByName("News"), 10);
	EXPECT_EQ(*tv.GetChannelName(10), "News");
}

TEST(TVSetNamesTest, DeleteName)
{
	TVSet tv;
	tv.TurnOn();

	tv.SetChannelName(3, "MTV");

	EXPECT_FALSE(tv.DeleteChannelName("MTV"));

	EXPECT_FALSE(tv.GetChannelName(3).has_value());
	EXPECT_FALSE(tv.GetChannelByName("MTV").has_value());

	EXPECT_TRUE(tv.DeleteChannelName("MTV"));
}

TEST(TVSetNamesTest, SelectByStringName)
{
	TVSet tv;
	tv.TurnOn();

	tv.SetChannelName(50, "Sports");

	EXPECT_FALSE(tv.SelectChannelByName(std::string("Sports")));
	EXPECT_EQ(tv.GetChannel(), 50);

	EXPECT_TRUE(tv.SelectChannelByName(std::string("Unknown")));
	EXPECT_EQ(tv.GetChannel(), 50);
}

TEST(TVSetNamesTest, ComplexNamesWithSpaces)
{
	TVSet tv;
	tv.TurnOn();

	EXPECT_FALSE(tv.SetChannelName(7, "National Geographic"));
	EXPECT_FALSE(tv.SetChannelName(10, "BBC World News"));

	EXPECT_EQ(*tv.GetChannelName(7), "National Geographic");
	EXPECT_EQ(*tv.GetChannelName(10), "BBC World News");

	EXPECT_EQ(*tv.GetChannelByName("National Geographic"), 7);
	EXPECT_EQ(*tv.GetChannelByName("BBC World News"), 10);

	EXPECT_FALSE(tv.SelectChannelByName(std::string("National Geographic")));
	EXPECT_EQ(tv.GetChannel(), 7);
}

TEST(TVSetNamesTest, RuNamesWithSpaces)
{
	TVSet tv;
	tv.TurnOn();

	EXPECT_FALSE(tv.SetChannelName(7, "Нэйшнл географик"));
	EXPECT_FALSE(tv.SetChannelName(10, "ББС ворд нюс"));

	EXPECT_EQ(*tv.GetChannelName(7), "Нэйшнл географик");
	EXPECT_EQ(*tv.GetChannelName(10), "ББС ворд нюс");

	EXPECT_EQ(*tv.GetChannelByName("Нэйшнл географик"), 7);
	EXPECT_EQ(*tv.GetChannelByName("ББС ворд нюс"), 10);

	EXPECT_FALSE(tv.SelectChannelByName(std::string("Нэйшнл географик")));
	EXPECT_EQ(tv.GetChannel(), 7);
}

TEST(TVSetNamesTest, ReassignNameFromCurrentChannel)
{
	TVSet tv;
	tv.TurnOn();

	tv.SetChannelName(1, "OldName");
	tv.SetChannelName(1, "NewName");

	EXPECT_FALSE(tv.GetChannelByName("OldName").has_value());
	EXPECT_EQ(*tv.GetChannelName(1), "NewName");
	EXPECT_EQ(*tv.GetChannelByName("NewName"), 1);
}

std::string RunCommand(const std::string& input)
{
	TVSet tv;
	std::istringstream iss(input);
	std::ostringstream oss;

	RemoteControl remote(tv, iss, oss);
	remote.HandleCommand();

	return oss.str();
}

std::string RunCommands(const std::string& input)
{
	TVSet tv;
	std::istringstream iss(input);
	std::ostringstream oss;

	RemoteControl remote(tv, iss, oss);

	while (iss.good() && !iss.eof())
	{
		remote.HandleCommand();
	}

	return oss.str();
}

TEST(RemoteControlTest, TurnOnCommand)
{
	const std::string output = RunCommand("TurnOn\n");
	EXPECT_EQ(output, "TV is turned on\n");
}

TEST(RemoteControlTest, TurnOffCommand)
{
	const std::string output = RunCommands("TurnOn\nTurnOff\n");
	EXPECT_TRUE(output.find("TV is turned off\n") != std::string::npos);
}

TEST(RemoteControlTest, InfoWhenOff)
{
	const std::string output = RunCommand("Info\n");
	EXPECT_EQ(output, "TV is turned off\n");
}

TEST(RemoteControlTest, InfoWhenOn)
{
	const std::string output = RunCommands("TurnOn\nInfo\n");
	EXPECT_TRUE(output.find("TV is turned on\n") != std::string::npos);
	EXPECT_TRUE(output.find("Channel is: 1\n") != std::string::npos);
}

TEST(RemoteControlTest, SelectChannelByNumber)
{
	const std::string output = RunCommands("TurnOn\nSelectChannel 42\nInfo\n");
	EXPECT_TRUE(output.find("Channel switched to: 42\n") != std::string::npos);
	EXPECT_TRUE(output.find("Channel is: 42\n") != std::string::npos);
}

TEST(RemoteControlTest, SelectChannelInvalidNumber)
{
	const std::string output = RunCommands("TurnOn\nSelectChannel 150\nInfo\n");
	EXPECT_TRUE(output.find("ERROR\n") != std::string::npos);
	EXPECT_TRUE(output.find("Channel is: 1\n") != std::string::npos);
}

TEST(RemoteControlTest, UnknownCommand)
{
	std::string output = RunCommand("UnknownCommand\n");
}

TEST(RemoteControlTest, SelectPreviousChannelSuccess)
{
	const std::string output = RunCommands(
		"TurnOn\n"
		"SelectChannel 5\n"
		"SelectChannel 10\n"
		"SelectPreviousChannel\n"
		"Info\n");
	EXPECT_TRUE(output.find("Switched to previous channel\n") != std::string::npos);
	EXPECT_TRUE(output.find("Channel is: 5\n") != std::string::npos);
}

TEST(RemoteControlTest, SelectPreviousChannelWhenOff)
{
	const std::string output = RunCommands(
		"TurnOn\n"
		"SelectChannel 10\n"
		"TurnOff\n"
		"SelectPreviousChannel\n");
	EXPECT_TRUE(output.find("ERROR\n") != std::string::npos);
}

TEST(RemoteControlTest, SelectPreviousChannelAfterPowerCycle)
{
	const std::string output = RunCommands(
		"TurnOn\n"
		"SelectChannel 10\n"
		"TurnOff\n"
		"SelectPreviousChannel\n"
		"TurnOn\n"
		"SelectPreviousChannel\n"
		"Info\n");
	EXPECT_TRUE(output.find("ERROR\n") != std::string::npos);
	EXPECT_TRUE(output.find("Switched to previous channel\n") != std::string::npos);
	EXPECT_TRUE(output.find("Channel is: 1\n") != std::string::npos);
}

TEST(RemoteControlTest, SetChannelName)
{
	const std::string output = RunCommands(
		"TurnOn\n"
		"SetChannelName 5 ORT\n"
		"Info\n");
	EXPECT_TRUE(output.find("Channel name set: 5 - ORT\n") != std::string::npos);
	EXPECT_TRUE(output.find("5 - ORT\n") != std::string::npos);
}

TEST(RemoteControlTest, SelectChannelByName)
{
	const std::string output = RunCommands(
		"TurnOn\n"
		"SetChannelName 7 Discovery\n"
		"SelectChannel Discovery\n"
		"Info\n");
	EXPECT_TRUE(output.find("Channel switched to: Discovery\n") != std::string::npos);
	EXPECT_TRUE(output.find("Channel is: 7\n") != std::string::npos);
}

TEST(RemoteControlTest, SelectChannelByNameNotFound)
{
	const std::string output = RunCommands(
		"TurnOn\n"
		"SelectChannel UnknownChannel\n");
	EXPECT_TRUE(output.find("ERROR\n") != std::string::npos);
}

TEST(RemoteControlTest, GetChannelName)
{
	const std::string output = RunCommands(
		"TurnOn\n"
		"SetChannelName 3 MTV\n"
		"GetChannelName 3\n");
	EXPECT_TRUE(output.find("Channel 3 name: MTV\n") != std::string::npos);
}

TEST(RemoteControlTest, GetChannelByName)
{
	const std::string output = RunCommands(
		"TurnOn\n"
		"SetChannelName 3 MTV\n"
		"GetChannelByName MTV\n");
	EXPECT_TRUE(output.find("Channel for name MTV: 3\n") != std::string::npos);
}

TEST(RemoteControlTest, DeleteChannelName)
{
	const std::string output = RunCommands(
		"TurnOn\n"
		"SetChannelName 3 MTV\n"
		"DeleteChannelName MTV\n"
		"GetChannelByName MTV\n");
	EXPECT_TRUE(output.find("Channel name deleted: MTV\n") != std::string::npos);
	EXPECT_TRUE(output.find("ERROR\n") != std::string::npos);
}

TEST(RemoteControlTest, ChannelNamesWithSpaces)
{
	const std::string output = RunCommands(
		"TurnOn\n"
		"SetChannelName 10 National Geographic\n"
		"SelectChannel National Geographic\n"
		"Info\n");
	EXPECT_TRUE(output.find("Channel name set: 10 - National Geographic\n") != std::string::npos);
	EXPECT_TRUE(output.find("Channel switched to: National Geographic\n") != std::string::npos);
	EXPECT_TRUE(output.find("10 - National Geographic\n") != std::string::npos);
}

TEST(RemoteControlTest, SelectChannelMissingArgument)
{
	const std::string output = RunCommands("TurnOn\nSelectChannel\n");
	EXPECT_TRUE(output.find("ERROR\n") != std::string::npos);
}

TEST(RemoteControlTest, SetChannelNameMissingArgument)
{
	const std::string output = RunCommands("TurnOn\nSetChannelName 5\n");
	EXPECT_TRUE(output.find("ERROR\n") != std::string::npos);
}

TEST(RemoteControlTest, EmptyChannelName)
{
	const std::string output = RunCommands("TurnOn\nSetChannelName 5 \n");
	EXPECT_TRUE(output.find("ERROR\n") != std::string::npos);
}