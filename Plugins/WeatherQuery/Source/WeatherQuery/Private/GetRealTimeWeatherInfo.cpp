// Fill out your copyright notice in the Description page of Project Settings.


#include "GetRealTimeWeatherInfo.h"

FWeatherInfo WeatherData;
FCityInfo CityInfo;
FString CityUrlID = "";
FString TempWeatherJsonString = "";
FString TempCityJsonString = "";
//ECityNameType CityNameType;

void UGetRealTimeWeatherInfo::GetWeatherInfo(const FString& City, const FString& key, bool isFreeVersion, enum ECityNameType cityname, FWeatherInfo& WeatherInfo, FCityInfo& CityInfoData, FString& WeatherJsonString, FString& CityJsonString, bool& isSuccess)
{

	FString WeatherApiUrl = "";
	FString CityApiUrl = "";
	
	if (City != "" && key != "")
	{

		switch (cityname)	/* 和风城市ID查询API链接 https://blog.csdn.net/Ang_ie/article/details/130093273*/
		{
		case ECityNameType::isID:

			CityUrlID = City;

			CityApiUrl = "https://geoapi.qweather.com/v2/city/lookup?location=" + CityUrlID + "&key=" + key;

			break;

		case ECityNameType::isEnglish:

			CityApiUrl = "https://geoapi.qweather.com/v2/city/lookup?location=" + City + "&key=" + key;

			break;

		case ECityNameType::isChinese:

			CityApiUrl = "https://geoapi.qweather.com/v2/city/lookup?location=" + FGenericPlatformHttp::UrlEncode(City) + "&key=" + key;

			break;

		default:
			break;

		}

		//TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();----老版本request方法，4.26版本以前使用
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> RequestCity = FHttpModule::Get().CreateRequest();

		RequestCity->OnProcessRequestComplete().BindStatic(&UGetRealTimeWeatherInfo::OnCityInfoResponseReceived);
		RequestCity->SetURL(CityApiUrl);
		RequestCity->SetVerb("GET");
		RequestCity->SetHeader("Content-Type", TEXT("application/json;charset=UTF-8"));
		RequestCity->ProcessRequest();

		CityInfoData = CityInfo;

		//和风天气API链接
		if (isFreeVersion)
		{
			WeatherApiUrl = "https://devapi.qweather.com/v7/weather/now?location=" + CityUrlID + "&key=" + key;
		}
		else
		{
			WeatherApiUrl = "https://api.qweather.com/v7/weather/now?location=" + CityUrlID + "&key=" + key;
		}

		//TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();----老版本request方法，4.26版本以前使用
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

		Request->OnProcessRequestComplete().BindStatic(&UGetRealTimeWeatherInfo::OnWeatherInfoResponseReceived);
		Request->SetURL(WeatherApiUrl);
		Request->SetVerb("GET");
		Request->SetHeader("Content-Type", TEXT("application/json;charset=UTF-8"));
		Request->ProcessRequest();

		WeatherInfo = WeatherData;
		WeatherJsonString = TempWeatherJsonString;
		CityJsonString = TempCityJsonString;
		isSuccess = true;
	}
	else
	{
		isSuccess = false;
	}
}


void UGetRealTimeWeatherInfo::OnWeatherInfoResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString JsonStr;
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		TSharedPtr<FJsonObject> Now = JsonObject->GetObjectField("now");

		WeatherData.Temperature = FCString::Atof(*Now->GetStringField("temp"));
		WeatherData.FeelsLike = FCString::Atof(*Now->GetStringField("feelsLike"));
		WeatherData.Icon = FCString::Atoi(*Now->GetStringField("icon"));
		WeatherData.WeatherText = Now->GetStringField("text");
		WeatherData.Wind360 = FCString::Atof(*Now->GetStringField("wind360"));
		WeatherData.WindDir = Now->GetStringField("windDir");
		WeatherData.WindScale = FCString::Atof(*Now->GetStringField("windScale"));
		WeatherData.WindSpeed = FCString::Atof(*Now->GetStringField("windSpeed"));
		WeatherData.Humidity = FCString::Atof(*Now->GetStringField("humidity"));
		WeatherData.Precip = FCString::Atof(*Now->GetStringField("precip"));
		WeatherData.Pressure = FCString::Atof(*Now->GetStringField("pressure"));
		WeatherData.WeatherVisibility = FCString::Atof(*Now->GetStringField("vis"));
		WeatherData.CloudCover = FCString::Atof(*Now->GetStringField("cloud"));
		WeatherData.Dew = FCString::Atof(*Now->GetStringField("dew"));

		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);//JSobject转string*/
		TempWeatherJsonString = JsonStr;
	}
}

void UGetRealTimeWeatherInfo::OnCityInfoResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString JsonStr;
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{

		TArray<TSharedPtr<FJsonValue>> CityDataArray = JsonObject->GetArrayField("location");
		TSharedPtr<FJsonObject> FirstCityData = CityDataArray[0]->AsObject();

		CityInfo.CityName = FirstCityData->GetStringField("name");
		CityInfo.CityID = FirstCityData->GetStringField("id");
		CityInfo.CityLat = FCString::Atof(*FirstCityData->GetStringField("lat"));
		CityInfo.CityLon = FCString::Atof(*FirstCityData->GetStringField("lon"));
		CityInfo.CityCountry = FirstCityData->GetStringField("country");
		CityInfo.CityTimeZone = FirstCityData->GetStringField("tz");
		CityInfo.UTCoffset = FirstCityData->GetStringField("utcOffset");
		CityInfo.IsDst = bool(*FirstCityData->GetStringField("lat"));
		CityInfo.Type = FirstCityData->GetStringField("type");
		CityUrlID = CityInfo.CityID;

		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);//JSobject转string*/
		TempCityJsonString = JsonStr;
	}
}


