#include "../src/TVSet.h"
#include <gtest/gtest.h>
#include <optional>
#include <string>

TEST(TVSetBasicTest, InitiallyTurnedOff)
{
	const CTVSet tv;
	EXPECT_FALSE(tv.IsTurnedOn());
	EXPECT_EQ(tv.GetChannel(), 0);
}

TEST(TVSetBasicTest, TurnOnFirstTimeStartsAtChannelOne)
{
	CTVSet tv;
	tv.TurnOn();
	EXPECT_TRUE(tv.IsTurnedOn());
	EXPECT_EQ(tv.GetChannel(), 1);
}

TEST(TVSetBasicTest, TurnOffRemembersLastChannelForNextTurnOn)
{
	CTVSet tv;
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
	CTVSet tv;
	EXPECT_FALSE(tv.SelectChannelByNumber(50));
	EXPECT_EQ(tv.GetChannel(), 0);
}

TEST(TVSetBasicTest, SelectValidChannelsRange)
{
	CTVSet tv;
	tv.TurnOn();

	EXPECT_TRUE(tv.SelectChannelByNumber(1));
	EXPECT_EQ(tv.GetChannel(), 1);

	EXPECT_TRUE(tv.SelectChannelByNumber(99));
	EXPECT_EQ(tv.GetChannel(), 99);

	EXPECT_TRUE(tv.SelectChannelByNumber(CTVSet::MIN_CHANNEL));
	EXPECT_TRUE(tv.SelectChannelByNumber(CTVSet::MAX_CHANNEL));
}

TEST(TVSetBasicTest, SelectInvalidChannelFailsAndDoesNotChange)
{
	CTVSet tv;
	tv.TurnOn();
	tv.SelectChannelByNumber(5);

	EXPECT_FALSE(tv.SelectChannelByNumber(0));
	EXPECT_FALSE(tv.SelectChannelByNumber(100));
	EXPECT_FALSE(tv.SelectChannelByNumber(-1));

	EXPECT_EQ(tv.GetChannel(), 5);
}

TEST(TVSetBasicTest, InfoStateWhenOff)
{
	CTVSet tv;

	EXPECT_FALSE(tv.IsTurnedOn());
	EXPECT_EQ(tv.GetChannel(), 0);
}

TEST(TVSetPrevChannelTest, FailsWhenTurnedOff)
{
	CTVSet tv;
	tv.TurnOn();
	tv.SelectChannelByNumber(10);
	tv.TurnOff();

	EXPECT_FALSE(tv.SelectPreviousChannel());
}

TEST(TVSetPrevChannelTest, FailsIfChannelNeverChanged)
{
	CTVSet tv;
	tv.TurnOn();

	EXPECT_FALSE(tv.SelectPreviousChannel());
	EXPECT_EQ(tv.GetChannel(), 1);
}

TEST(TVSetPrevChannelTest, SwitchesBackAndForthSimple)
{
	CTVSet tv;
	tv.TurnOn();

	tv.SelectChannelByNumber(2);
	tv.SelectChannelByNumber(5);

	EXPECT_EQ(tv.GetChannel(), 5);

	EXPECT_TRUE(tv.SelectPreviousChannel());
	EXPECT_EQ(tv.GetChannel(), 2);

	EXPECT_TRUE(tv.SelectPreviousChannel());
	EXPECT_EQ(tv.GetChannel(), 5);
}

TEST(TVSetPrevChannelTest, PreservesHistoryAfterPowerCycle)
{
	CTVSet tv;

	tv.TurnOn();
	tv.SelectChannelByNumber(10);

	tv.TurnOff();

	EXPECT_FALSE(tv.SelectPreviousChannel());

	tv.TurnOn();

	EXPECT_EQ(tv.GetChannel(), 10);

	EXPECT_TRUE(tv.SelectPreviousChannel());
	EXPECT_EQ(tv.GetChannel(), 1);
}

// ============================================================================
// БОНУС: ИМЕНА КАНАЛОВ (+40 баллов)
// ============================================================================

TEST(TVSetNamesTest, OperationsOnlyWhenOn)
{
	CTVSet tv;
	// Все операции с именами запрещены на выключенном ТВ
	EXPECT_FALSE(tv.SetChannelName(5, "ORT"));
	EXPECT_FALSE(tv.DeleteChannelName("MTV"));
	EXPECT_FALSE(tv.GetChannelName(5).has_value());
	EXPECT_FALSE(tv.GetChannelByName("ORT").has_value());
	EXPECT_FALSE(tv.SelectChannelByName(std::string("ORT")));
}

TEST(TVSetNamesTest, SetAndGetBidirectional)
{
	CTVSet tv;
	tv.TurnOn();

	EXPECT_TRUE(tv.SetChannelName(5, "ORT"));

	// Проверка: Номер -> Имя
	auto name = tv.GetChannelName(5);
	ASSERT_TRUE(name.has_value());
	EXPECT_EQ(*name, "ORT");

	// Проверка: Имя -> Номер
	auto ch = tv.GetChannelByName("ORT");
	ASSERT_TRUE(ch.has_value());
	EXPECT_EQ(*ch, 5);
}

TEST(TVSetNamesTest, NormalizeSpaces)
{
	CTVSet tv;
	tv.TurnOn();

	// Вход: пробелы в начале, конце и множественные внутри
	EXPECT_TRUE(tv.SetChannelName(1, "  Discovery   Channel  "));

	// Внутри должно быть нормализовано: "Discovery Channel"
	auto name = tv.GetChannelName(1);
	ASSERT_TRUE(name.has_value());
	EXPECT_EQ(*name, "Discovery Channel");

	// Поиск также должен работать с "грязным" вводом
	auto ch = tv.GetChannelByName(" Discovery  Channel ");
	ASSERT_TRUE(ch.has_value());
	EXPECT_EQ(*ch, 1);
}

TEST(TVSetNamesTest, RejectEmptyAndSpaceOnlyNames)
{
	CTVSet tv;
	tv.TurnOn();

	EXPECT_FALSE(tv.SetChannelName(1, ""));
	EXPECT_FALSE(tv.SetChannelName(1, "   "));
	EXPECT_FALSE(tv.SetChannelName(1, "\t\n"));
}

TEST(TVSetNamesTest, OneToOneMappingReassignment)
{
	CTVSet tv;
	tv.TurnOn();

	// Назначаем имя "News" каналу 7
	tv.SetChannelName(7, "News");
	EXPECT_EQ(*tv.GetChannelByName("News"), 7);

	// Переназначаем то же имя каналу 10
	tv.SetChannelName(10, "News");

	// У канала 7 имя должно пропасть
	EXPECT_FALSE(tv.GetChannelName(7).has_value());

	// У канала 10 должно появиться
	EXPECT_EQ(*tv.GetChannelByName("News"), 10);
	EXPECT_EQ(*tv.GetChannelName(10), "News");
}

TEST(TVSetNamesTest, DeleteName)
{
	CTVSet tv;
	tv.TurnOn();

	tv.SetChannelName(3, "MTV");

	EXPECT_TRUE(tv.DeleteChannelName("MTV"));

	// Связи разорваны
	EXPECT_FALSE(tv.GetChannelName(3).has_value());
	EXPECT_FALSE(tv.GetChannelByName("MTV").has_value());

	// Повторное удаление должно вернуть false
	EXPECT_FALSE(tv.DeleteChannelName("MTV"));
}

TEST(TVSetNamesTest, SelectByStringName)
{
	CTVSet tv;
	tv.TurnOn();

	tv.SetChannelName(50, "Sports");

	// Перегрузка SelectChannelByNumber(string)
	EXPECT_TRUE(tv.SelectChannelByName(std::string("Sports")));
	EXPECT_EQ(tv.GetChannel(), 50);

	// Несуществующее имя
	EXPECT_FALSE(tv.SelectChannelByName(std::string("Unknown")));
	EXPECT_EQ(tv.GetChannel(), 50); // Канал не изменился
}

TEST(TVSetNamesTest, ComplexNamesWithSpaces)
{
	CTVSet tv;
	tv.TurnOn();

	// Имена из нескольких слов (как в примере README)
	EXPECT_TRUE(tv.SetChannelName(7, "National Geographic"));
	EXPECT_TRUE(tv.SetChannelName(10, "BBC World News"));

	EXPECT_EQ(*tv.GetChannelName(7), "National Geographic");
	EXPECT_EQ(*tv.GetChannelName(10), "BBC World News");

	EXPECT_EQ(*tv.GetChannelByName("National Geographic"), 7);
	EXPECT_EQ(*tv.GetChannelByName("BBC World News"), 10);

	// Переключение по длинному имени
	EXPECT_TRUE(tv.SelectChannelByName(std::string("National Geographic")));
	EXPECT_EQ(tv.GetChannel(), 7);
}

TEST(TVSetNamesTest, ReassignNameFromCurrentChannel)
{
	CTVSet tv;
	tv.TurnOn();

	// Канал 1 имеет имя "OldName"
	tv.SetChannelName(1, "OldName");

	// Назначаем каналу 1 новое имя "NewName"
	tv.SetChannelName(1, "NewName");

	// Старое имя должно исчезнуть
	EXPECT_FALSE(tv.GetChannelByName("OldName").has_value());

	// Новое имя должно работать
	EXPECT_EQ(*tv.GetChannelName(1), "NewName");
	EXPECT_EQ(*tv.GetChannelByName("NewName"), 1);
}