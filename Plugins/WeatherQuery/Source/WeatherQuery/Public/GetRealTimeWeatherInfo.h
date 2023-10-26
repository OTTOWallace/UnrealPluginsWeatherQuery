// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HTTP.h"
#include "GetRealTimeWeatherInfo.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FWeatherInfo
{
	GENERATED_USTRUCT_BODY()


	/* 温度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	float Temperature;

	/* 体感温度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	float FeelsLike;

	/* 天气状况的图标代码 https://icons.qweather.com*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	int Icon;

	/* 天气状况的文字描述，包括阴晴雨雪等天气状态的描述 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	FString WeatherText;

	/* 风向360角度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	float Wind360;

	/* 风向 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	FString WindDir;

	/* 风力等级 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	float WindScale;

	/*  风速，公里/小时 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	float WindSpeed;

	/*  相对湿度，百分比数值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	float Humidity;

	/*  当前小时累计降水量，默认单位：毫米 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	float Precip;

	/*  大气压强，默认单位：百帕 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	float Pressure;

	/*  能见度，默认单位：公里 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	float WeatherVisibility;

	/*  云量，百分比数值。可能为空 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	float CloudCover;

	/*  露点温度。可能为空 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	float Dew;
};

USTRUCT(BlueprintType)
struct FCityInfo
{
	GENERATED_USTRUCT_BODY()


	/* 地区/城市名称 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	FString CityName;

	/* 城市ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	FString CityID;

	/* 城市纬度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	float CityLat;

	/* 城市经度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	float CityLon;

	/* 地区/城市所属国家名称 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	FString CityCountry;

	/* 地区/城市所在时区 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	FString CityTimeZone;

	/* 地区/城市目前与UTC时间偏移的小时数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	FString UTCoffset;

	/* 地区/城市是否当前处于夏令时。1 表示当前处于夏令时，0 表示当前不是夏令时 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	bool IsDst;

	/* 地区/城市的属性 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeatherInfo")
	FString Type;
};


UENUM(BlueprintType)
enum class ECityNameType : uint8
{
	isID ,
	isEnglish,
	isChinese,
};

UCLASS()
class WEATHERQUERY_API UGetRealTimeWeatherInfo : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/* 请求 */
	UFUNCTION(BlueprintCallable, Category = "WeatherInfo")
	static void GetWeatherInfo(const FString& City, const FString& key, bool isFreeVersion, enum ECityNameType cityname, FWeatherInfo& WeatherInfo, FCityInfo& CityInfoData, FString& WeatherJsonString, FString& CityJsonString, bool& isSuccess);

	static void OnWeatherInfoResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	static void OnCityInfoResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

};
