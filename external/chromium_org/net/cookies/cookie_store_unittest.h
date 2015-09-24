// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_COOKIES_COOKIE_STORE_UNITTEST_H_
#define NET_COOKIES_COOKIE_STORE_UNITTEST_H_

#include "base/bind.h"
#include "base/message_loop/message_loop.h"
#include "base/strings/string_tokenizer.h"
#include "base/threading/thread.h"
#include "net/cookies/cookie_monster.h"
#include "net/cookies/cookie_store.h"
#include "net/cookies/cookie_store_test_callbacks.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"


namespace net {

using base::Thread;

const int kTimeout = 1000;

const char kUrlFtp[] = "ftp://ftp.google.izzle/";
const char kUrlGoogle[] = "http://www.google.izzle";
const char kUrlGoogleFoo[] = "http://www.google.izzle/foo";
const char kUrlGoogleBar[] = "http://www.google.izzle/bar";
const char kUrlGoogleSecure[] = "https://www.google.izzle";
const char kValidCookieLine[] = "A=B; path=/";
const char kValidDomainCookieLine[] = "A=B; path=/; domain=google.izzle";


template <class CookieStoreTestTraits>
class CookieStoreTest : public testing::Test {
 protected:
  CookieStoreTest()
      : url_google_(kUrlGoogle),
        url_google_secure_(kUrlGoogleSecure),
        url_google_foo_(kUrlGoogleFoo),
        url_google_bar_(kUrlGoogleBar) {
    
    
    if (!base::MessageLoop::current())
      message_loop_.reset(new base::MessageLoop);
    weak_factory_.reset(new base::WeakPtrFactory<base::MessageLoop>(
        base::MessageLoop::current()));
  }

  
  
  

  std::string GetCookies(CookieStore* cs, const GURL& url) {
    DCHECK(cs);
    CookieOptions options;
    if (!CookieStoreTestTraits::supports_http_only)
      options.set_include_httponly();
    StringResultCookieCallback callback;
    cs->GetCookiesWithOptionsAsync(
        url, options,
        base::Bind(&StringResultCookieCallback::Run,
                   base::Unretained(&callback)));
    RunFor(kTimeout);
    EXPECT_TRUE(callback.did_run());
    return callback.result();
  }

  std::string GetCookiesWithOptions(CookieStore* cs,
                                    const GURL& url,
                                    const CookieOptions& options) {
    DCHECK(cs);
    StringResultCookieCallback callback;
    cs->GetCookiesWithOptionsAsync(
        url, options, base::Bind(&StringResultCookieCallback::Run,
                                 base::Unretained(&callback)));
    RunFor(kTimeout);
    EXPECT_TRUE(callback.did_run());
    return callback.result();
  }

  bool SetCookieWithOptions(CookieStore* cs,
                            const GURL& url,
                            const std::string& cookie_line,
                            const CookieOptions& options) {
    DCHECK(cs);
    BoolResultCookieCallback callback;
    cs->SetCookieWithOptionsAsync(
        url, cookie_line, options,
        base::Bind(&BoolResultCookieCallback::Run,
                   base::Unretained(&callback)));
    RunFor(kTimeout);
    EXPECT_TRUE(callback.did_run());
    return callback.result();
  }

  bool SetCookieWithServerTime(CookieStore* cs,
                               const GURL& url,
                               const std::string& cookie_line,
                               const base::Time& server_time) {
    CookieOptions options;
    if (!CookieStoreTestTraits::supports_http_only)
      options.set_include_httponly();
    options.set_server_time(server_time);
    return SetCookieWithOptions(cs, url, cookie_line, options);
  }

  bool SetCookie(CookieStore* cs,
                 const GURL& url,
                 const std::string& cookie_line) {
    CookieOptions options;
    if (!CookieStoreTestTraits::supports_http_only)
      options.set_include_httponly();
    return SetCookieWithOptions(cs, url, cookie_line, options);
  }

  void DeleteCookie(CookieStore* cs,
                    const GURL& url,
                    const std::string& cookie_name) {
    DCHECK(cs);
    NoResultCookieCallback callback;
    cs->DeleteCookieAsync(
        url, cookie_name,
        base::Bind(&NoResultCookieCallback::Run, base::Unretained(&callback)));
    RunFor(kTimeout);
    EXPECT_TRUE(callback.did_run());
  }

  int DeleteCreatedBetween(CookieStore* cs,
                            const base::Time& delete_begin,
                            const base::Time& delete_end) {
    DCHECK(cs);
    IntResultCookieCallback callback;
    cs->DeleteAllCreatedBetweenAsync(
        delete_begin, delete_end,
        base::Bind(&IntResultCookieCallback::Run, base::Unretained(&callback)));
    RunFor(kTimeout);
    EXPECT_TRUE(callback.did_run());
    return callback.result();
  }

  int DeleteSessionCookies(CookieStore* cs) {
    DCHECK(cs);
    IntResultCookieCallback callback;
    cs->DeleteSessionCookiesAsync(
        base::Bind(&IntResultCookieCallback::Run, base::Unretained(&callback)));
    RunFor(kTimeout);
    EXPECT_TRUE(callback.did_run());
    return callback.result();
  }

  void RunFor(int ms) {
    
    base::MessageLoop::current()->PostDelayedTask(
        FROM_HERE,
        base::Bind(&base::MessageLoop::Quit, weak_factory_->GetWeakPtr()),
        base::TimeDelta::FromMilliseconds(ms));
    base::MessageLoop::current()->Run();
    weak_factory_->InvalidateWeakPtrs();
  }

  scoped_refptr<CookieStore> GetCookieStore() {
    return CookieStoreTestTraits::Create();
  }

  
  void MatchCookieLines(const std::string& line1, const std::string& line2) {
    EXPECT_EQ(TokenizeCookieLine(line1), TokenizeCookieLine(line2));
  }

  
  void MatchCookieLineWithTimeout(CookieStore* cs,
                                  const GURL& url,
                                  const std::string& line) {
    std::string cookies = GetCookies(cs, url);
    bool matched = (TokenizeCookieLine(line) == TokenizeCookieLine(cookies));
    base::Time polling_end_date = base::Time::Now() +
        base::TimeDelta::FromMilliseconds(
            CookieStoreTestTraits::creation_time_granularity_in_ms);

    while (!matched &&  base::Time::Now() <= polling_end_date) {
      base::PlatformThread::Sleep(base::TimeDelta::FromMilliseconds(10));
      cookies = GetCookies(cs, url);
      matched = (TokenizeCookieLine(line) == TokenizeCookieLine(cookies));
    }

    EXPECT_TRUE(matched) << "\"" << cookies
                         << "\" does not match \"" << line << "\"";
  }

  GURL url_google_;
  GURL url_google_secure_;
  GURL url_google_foo_;
  GURL url_google_bar_;

  scoped_ptr<base::WeakPtrFactory<base::MessageLoop> > weak_factory_;
  scoped_ptr<base::MessageLoop> message_loop_;

 private:
  
  std::set<std::string> TokenizeCookieLine(const std::string& line) {
    std::set<std::string> tokens;
    base::StringTokenizer tokenizer(line, " ;");
    while (tokenizer.GetNext())
      EXPECT_TRUE(tokens.insert(tokenizer.token()).second);
    return tokens;
  }
};

TYPED_TEST_CASE_P(CookieStoreTest);

TYPED_TEST_P(CookieStoreTest, TypeTest) {
  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  EXPECT_EQ(cs->GetCookieMonster(),
            (TypeParam::is_cookie_monster) ?
                static_cast<CookieMonster*>(cs.get()) : NULL);
}

TYPED_TEST_P(CookieStoreTest, DomainTest) {
  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  EXPECT_TRUE(this->SetCookie(cs.get(), this->url_google_, "A=B"));
  this->MatchCookieLines("A=B", this->GetCookies(cs.get(), this->url_google_));
  EXPECT_TRUE(this->SetCookie(
      cs.get(), this->url_google_, "C=D; domain=.google.izzle"));
  this->MatchCookieLines("A=B; C=D",
                         this->GetCookies(cs.get(), this->url_google_));

  
  
  this->MatchCookieLines(
      "C=D", this->GetCookies(cs.get(), GURL("http://foo.www.google.izzle")));

  
  EXPECT_TRUE(this->SetCookie(
      cs.get(), this->url_google_, "E=F; domain=.www.google.izzle"));
  this->MatchCookieLines("A=B; C=D; E=F",
                         this->GetCookies(cs.get(), this->url_google_));

  
  
  EXPECT_TRUE(this->SetCookie(
      cs.get(), this->url_google_, "G=H; domain=www.google.izzle"));
  this->MatchCookieLines("A=B; C=D; E=F; G=H",
                         this->GetCookies(cs.get(), this->url_google_));

  
  EXPECT_FALSE(
      this->SetCookie(cs.get(), this->url_google_, "I=J; domain=.izzle"));
  this->MatchCookieLines(std::string(),
                         this->GetCookies(cs.get(), GURL("http://a.izzle")));
  EXPECT_FALSE(this->SetCookie(
      cs.get(), this->url_google_, "K=L; domain=.bla.www.google.izzle"));
  this->MatchCookieLines(
      "C=D; E=F; G=H",
      this->GetCookies(cs.get(), GURL("http://bla.www.google.izzle")));
  this->MatchCookieLines("A=B; C=D; E=F; G=H",
                         this->GetCookies(cs.get(), this->url_google_));
}

TYPED_TEST_P(CookieStoreTest, DomainWithTrailingDotTest) {
  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  EXPECT_FALSE(this->SetCookie(
      cs.get(), this->url_google_, "a=1; domain=.www.google.com."));
  EXPECT_FALSE(this->SetCookie(
      cs.get(), this->url_google_, "b=2; domain=.www.google.com.."));
  this->MatchCookieLines(std::string(),
                         this->GetCookies(cs.get(), this->url_google_));
}

TYPED_TEST_P(CookieStoreTest, ValidSubdomainTest) {
  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  GURL url_abcd("http://a.b.c.d.com");
  GURL url_bcd("http://b.c.d.com");
  GURL url_cd("http://c.d.com");
  GURL url_d("http://d.com");

  EXPECT_TRUE(this->SetCookie(cs.get(), url_abcd, "a=1; domain=.a.b.c.d.com"));
  EXPECT_TRUE(this->SetCookie(cs.get(), url_abcd, "b=2; domain=.b.c.d.com"));
  EXPECT_TRUE(this->SetCookie(cs.get(), url_abcd, "c=3; domain=.c.d.com"));
  EXPECT_TRUE(this->SetCookie(cs.get(), url_abcd, "d=4; domain=.d.com"));

  this->MatchCookieLines("a=1; b=2; c=3; d=4",
                         this->GetCookies(cs.get(), url_abcd));
  this->MatchCookieLines("b=2; c=3; d=4", this->GetCookies(cs.get(), url_bcd));
  this->MatchCookieLines("c=3; d=4", this->GetCookies(cs.get(), url_cd));
  this->MatchCookieLines("d=4", this->GetCookies(cs.get(), url_d));

  
  EXPECT_TRUE(this->SetCookie(cs.get(), url_bcd, "X=bcd; domain=.b.c.d.com"));
  EXPECT_TRUE(this->SetCookie(cs.get(), url_bcd, "X=cd; domain=.c.d.com"));
  this->MatchCookieLines("b=2; c=3; d=4; X=bcd; X=cd",
                         this->GetCookies(cs.get(), url_bcd));
  this->MatchCookieLines("c=3; d=4; X=cd", this->GetCookies(cs.get(), url_cd));
}

TYPED_TEST_P(CookieStoreTest, InvalidDomainTest) {
  {
    scoped_refptr<CookieStore> cs(this->GetCookieStore());
    GURL url_foobar("http://foo.bar.com");

    
    EXPECT_FALSE(
        this->SetCookie(cs.get(), url_foobar, "a=1; domain=.yo.foo.bar.com"));

    EXPECT_FALSE(this->SetCookie(cs.get(), url_foobar, "b=2; domain=.foo.com"));
    EXPECT_FALSE(
        this->SetCookie(cs.get(), url_foobar, "c=3; domain=.bar.foo.com"));

    
    EXPECT_FALSE(
        this->SetCookie(cs.get(), url_foobar, "d=4; domain=.foo.bar.com.net"));

    
    EXPECT_FALSE(this->SetCookie(cs.get(), url_foobar, "e=5; domain=ar.com"));

    
    EXPECT_FALSE(this->SetCookie(cs.get(), url_foobar, "f=6; domain=."));
    EXPECT_FALSE(this->SetCookie(cs.get(), url_foobar, "g=7; domain=/"));
    EXPECT_FALSE(this->SetCookie(
        cs.get(), url_foobar, "h=8; domain=http://foo.bar.com"));
    EXPECT_FALSE(
        this->SetCookie(cs.get(), url_foobar, "i=9; domain=..foo.bar.com"));
    EXPECT_FALSE(
        this->SetCookie(cs.get(), url_foobar, "j=10; domain=..bar.com"));

    
    
    EXPECT_FALSE(this->SetCookie(
        cs.get(), url_foobar, "k=11; domain=.foo.bar.com?blah"));
    EXPECT_FALSE(this->SetCookie(
        cs.get(), url_foobar, "l=12; domain=.foo.bar.com/blah"));
    EXPECT_FALSE(
        this->SetCookie(cs.get(), url_foobar, "m=13; domain=.foo.bar.com:80"));
    EXPECT_FALSE(
        this->SetCookie(cs.get(), url_foobar, "n=14; domain=.foo.bar.com:"));
    EXPECT_FALSE(
        this->SetCookie(cs.get(), url_foobar, "o=15; domain=.foo.bar.com#sup"));

    this->MatchCookieLines(std::string(),
                           this->GetCookies(cs.get(), url_foobar));
  }

  {
    
    
    
    scoped_refptr<CookieStore> cs(this->GetCookieStore());
    GURL url_foocom("http://foo.com.com");
    EXPECT_FALSE(
        this->SetCookie(cs.get(), url_foocom, "a=1; domain=.foo.com.com.com"));
    this->MatchCookieLines(std::string(),
                           this->GetCookies(cs.get(), url_foocom));
  }
}

TYPED_TEST_P(CookieStoreTest, DomainWithoutLeadingDotTest) {
  {  
    scoped_refptr<CookieStore> cs(this->GetCookieStore());
    GURL url_hosted("http://manage.hosted.filefront.com");
    GURL url_filefront("http://www.filefront.com");
    EXPECT_TRUE(
        this->SetCookie(cs.get(), url_hosted, "sawAd=1; domain=filefront.com"));
    this->MatchCookieLines("sawAd=1", this->GetCookies(cs.get(), url_hosted));
    this->MatchCookieLines("sawAd=1",
                           this->GetCookies(cs.get(), url_filefront));
  }

  {  
    scoped_refptr<CookieStore> cs(this->GetCookieStore());
    GURL url("http://www.google.com");
    EXPECT_TRUE(this->SetCookie(cs.get(), url, "a=1; domain=www.google.com"));
    this->MatchCookieLines("a=1", this->GetCookies(cs.get(), url));
    this->MatchCookieLines(
        "a=1", this->GetCookies(cs.get(), GURL("http://sub.www.google.com")));
    this->MatchCookieLines(
        std::string(),
        this->GetCookies(cs.get(), GURL("http://something-else.com")));
  }
}

TYPED_TEST_P(CookieStoreTest, CaseInsensitiveDomainTest) {
    scoped_refptr<CookieStore> cs(this->GetCookieStore());
  GURL url("http://www.google.com");
  EXPECT_TRUE(this->SetCookie(cs.get(), url, "a=1; domain=.GOOGLE.COM"));
  EXPECT_TRUE(this->SetCookie(cs.get(), url, "b=2; domain=.wWw.gOOgLE.coM"));
  this->MatchCookieLines("a=1; b=2", this->GetCookies(cs.get(), url));
}

TYPED_TEST_P(CookieStoreTest, TestIpAddress) {
  GURL url_ip("http://1.2.3.4/weee");
  {
    scoped_refptr<CookieStore> cs(this->GetCookieStore());
    EXPECT_TRUE(this->SetCookie(cs.get(), url_ip, kValidCookieLine));
    this->MatchCookieLines("A=B", this->GetCookies(cs.get(), url_ip));
  }

  {  
    scoped_refptr<CookieStore> cs(this->GetCookieStore());
    EXPECT_FALSE(this->SetCookie(cs.get(), url_ip, "b=2; domain=.1.2.3.4"));
    EXPECT_FALSE(this->SetCookie(cs.get(), url_ip, "c=3; domain=.3.4"));
    this->MatchCookieLines(std::string(), this->GetCookies(cs.get(), url_ip));
    
    
    EXPECT_FALSE(this->SetCookie(cs.get(), url_ip, "b=2; domain=1.2.3.3"));
    this->MatchCookieLines(std::string(), this->GetCookies(cs.get(), url_ip));
    EXPECT_TRUE(this->SetCookie(cs.get(), url_ip, "b=2; domain=1.2.3.4"));
    this->MatchCookieLines("b=2", this->GetCookies(cs.get(), url_ip));
  }
}

TYPED_TEST_P(CookieStoreTest, TestNonDottedAndTLD) {
  {
    scoped_refptr<CookieStore> cs(this->GetCookieStore());
    GURL url("http://com/");
    
    EXPECT_TRUE(this->SetCookie(cs.get(), url, "a=1"));
    EXPECT_FALSE(this->SetCookie(cs.get(), url, "b=2; domain=.com"));
    EXPECT_FALSE(this->SetCookie(cs.get(), url, "c=3; domain=com"));
    this->MatchCookieLines("a=1", this->GetCookies(cs.get(), url));
    
    
    this->MatchCookieLines(
        std::string(),
        this->GetCookies(cs.get(), GURL("http://hopefully-no-cookies.com/")));
    if (TypeParam::supports_non_dotted_domains) {
      this->MatchCookieLines(std::string(),
                             this->GetCookies(cs.get(), GURL("http://.com/")));
    }
  }

  {
    
    scoped_refptr<CookieStore> cs(this->GetCookieStore());
    GURL url("http://com./index.html");
    if (TypeParam::supports_trailing_dots) {
      EXPECT_TRUE(this->SetCookie(cs.get(), url, "a=1"));
      this->MatchCookieLines("a=1", this->GetCookies(cs.get(), url));
      this->MatchCookieLines(
          std::string(),
          this->GetCookies(cs.get(),
                           GURL("http://hopefully-no-cookies.com./")));
    } else {
      EXPECT_FALSE(this->SetCookie(cs.get(), url, "a=1"));
    }
  }

  {  
    scoped_refptr<CookieStore> cs(this->GetCookieStore());
    GURL url("http://a.b");
    EXPECT_FALSE(this->SetCookie(cs.get(), url, "a=1; domain=.b"));
    EXPECT_FALSE(this->SetCookie(cs.get(), url, "b=2; domain=b"));
    this->MatchCookieLines(std::string(), this->GetCookies(cs.get(), url));
  }

  {  
    scoped_refptr<CookieStore> cs(this->GetCookieStore());
    GURL url("http://google.com");
    EXPECT_FALSE(this->SetCookie(cs.get(), url, "a=1; domain=.com"));
    EXPECT_FALSE(this->SetCookie(cs.get(), url, "b=2; domain=com"));
    this->MatchCookieLines(std::string(), this->GetCookies(cs.get(), url));
  }

  {  
    scoped_refptr<CookieStore> cs(this->GetCookieStore());
    GURL url("http://google.co.uk");
    EXPECT_FALSE(this->SetCookie(cs.get(), url, "a=1; domain=.co.uk"));
    EXPECT_FALSE(this->SetCookie(cs.get(), url, "b=2; domain=.uk"));
    this->MatchCookieLines(std::string(), this->GetCookies(cs.get(), url));
    this->MatchCookieLines(
        std::string(),
        this->GetCookies(cs.get(), GURL("http://something-else.co.uk")));
    this->MatchCookieLines(
        std::string(),
        this->GetCookies(cs.get(), GURL("http://something-else.uk")));
  }

  {  
    scoped_refptr<CookieStore> cs(this->GetCookieStore());
    GURL url("http://b");
    EXPECT_TRUE(this->SetCookie(cs.get(), url, "a=1"));
    EXPECT_FALSE(this->SetCookie(cs.get(), url, "b=2; domain=.b"));
    EXPECT_FALSE(this->SetCookie(cs.get(), url, "c=3; domain=b"));
    this->MatchCookieLines("a=1", this->GetCookies(cs.get(), url));
  }
}

TYPED_TEST_P(CookieStoreTest, TestHostEndsWithDot) {
  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  GURL url("http://www.google.com");
  GURL url_with_dot("http://www.google.com.");
  EXPECT_TRUE(this->SetCookie(cs.get(), url, "a=1"));
  this->MatchCookieLines("a=1", this->GetCookies(cs.get(), url));

  if (TypeParam::supports_trailing_dots) {
    
    
    EXPECT_FALSE(
        this->SetCookie(cs.get(), url, "b=2; domain=.www.google.com."));
    this->MatchCookieLines("a=1", this->GetCookies(cs.get(), url));

    EXPECT_TRUE(
        this->SetCookie(cs.get(), url_with_dot, "b=2; domain=.google.com."));
    this->MatchCookieLines("b=2", this->GetCookies(cs.get(), url_with_dot));
  } else {
    EXPECT_TRUE(this->SetCookie(cs.get(), url, "b=2; domain=.www.google.com."));
    EXPECT_FALSE(
        this->SetCookie(cs.get(), url_with_dot, "b=2; domain=.google.com."));
  }

  
  this->MatchCookieLines(
      std::string(),
      this->GetCookies(cs.get(), GURL("http://hopefully-no-cookies.com/")));
  this->MatchCookieLines(std::string(),
                         this->GetCookies(cs.get(), GURL("http://.com/")));
}

TYPED_TEST_P(CookieStoreTest, InvalidScheme) {
  if (!TypeParam::filters_schemes)
    return;

  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  EXPECT_FALSE(this->SetCookie(cs.get(), GURL(kUrlFtp), kValidCookieLine));
}

TYPED_TEST_P(CookieStoreTest, InvalidScheme_Read) {
  if (!TypeParam::filters_schemes)
    return;

  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  EXPECT_TRUE(
      this->SetCookie(cs.get(), GURL(kUrlGoogle), kValidDomainCookieLine));
  this->MatchCookieLines(std::string(),
                         this->GetCookies(cs.get(), GURL(kUrlFtp)));
}

TYPED_TEST_P(CookieStoreTest, PathTest) {
  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  std::string url("http://www.google.izzle");
  EXPECT_TRUE(this->SetCookie(cs.get(), GURL(url), "A=B; path=/wee"));
  this->MatchCookieLines("A=B", this->GetCookies(cs.get(), GURL(url + "/wee")));
  this->MatchCookieLines("A=B",
                         this->GetCookies(cs.get(), GURL(url + "/wee/")));
  this->MatchCookieLines("A=B",
                         this->GetCookies(cs.get(), GURL(url + "/wee/war")));
  this->MatchCookieLines(
      "A=B", this->GetCookies(cs.get(), GURL(url + "/wee/war/more/more")));
  if (!TypeParam::has_path_prefix_bug)
    this->MatchCookieLines(std::string(),
                           this->GetCookies(cs.get(), GURL(url + "/weehee")));
  this->MatchCookieLines(std::string(),
                         this->GetCookies(cs.get(), GURL(url + "/")));

  
  EXPECT_TRUE(this->SetCookie(cs.get(), GURL(url), "A=C; path="));
  this->MatchCookieLines("A=B; A=C",
                         this->GetCookies(cs.get(), GURL(url + "/wee")));
  this->MatchCookieLines("A=C", this->GetCookies(cs.get(), GURL(url + "/")));
}

TYPED_TEST_P(CookieStoreTest, EmptyExpires) {
  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  CookieOptions options;
  if (!TypeParam::supports_http_only)
    options.set_include_httponly();
  GURL url("http://www7.ipdl.inpit.go.jp/Tokujitu/tjkta.ipdl?N0000=108");
  std::string set_cookie_line =
      "ACSTM=20130308043820420042; path=/; domain=ipdl.inpit.go.jp; Expires=";
  std::string cookie_line = "ACSTM=20130308043820420042";

  this->SetCookieWithOptions(cs.get(), url, set_cookie_line, options);
  this->MatchCookieLines(cookie_line,
                         this->GetCookiesWithOptions(cs.get(), url, options));

  options.set_server_time(base::Time::Now() - base::TimeDelta::FromHours(1));
  this->SetCookieWithOptions(cs.get(), url, set_cookie_line, options);
  this->MatchCookieLines(cookie_line,
                         this->GetCookiesWithOptions(cs.get(), url, options));

  options.set_server_time(base::Time::Now() + base::TimeDelta::FromHours(1));
  this->SetCookieWithOptions(cs.get(), url, set_cookie_line, options);
  this->MatchCookieLines(cookie_line,
                         this->GetCookiesWithOptions(cs.get(), url, options));
}

TYPED_TEST_P(CookieStoreTest, HttpOnlyTest) {
  if (!TypeParam::supports_http_only)
    return;

  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  CookieOptions options;
  options.set_include_httponly();

  
  EXPECT_TRUE(this->SetCookieWithOptions(
      cs.get(), this->url_google_, "A=B; httponly", options));

  
  this->MatchCookieLines(std::string(),
                         this->GetCookies(cs.get(), this->url_google_));
  this->MatchCookieLines(
      "A=B", this->GetCookiesWithOptions(cs.get(), this->url_google_, options));

  
  EXPECT_FALSE(this->SetCookie(cs.get(), this->url_google_, "A=C"));
  this->MatchCookieLines(std::string(),
                         this->GetCookies(cs.get(), this->url_google_));
  this->MatchCookieLines(
      "A=B", this->GetCookiesWithOptions(cs.get(), this->url_google_, options));
  EXPECT_TRUE(
      this->SetCookieWithOptions(cs.get(), this->url_google_, "A=C", options));
  this->MatchCookieLines("A=C", this->GetCookies(cs.get(), this->url_google_));

  
  EXPECT_FALSE(this->SetCookie(cs.get(), this->url_google_, "B=A; httponly"));
  this->MatchCookieLines(
      "A=C", this->GetCookiesWithOptions(cs.get(), this->url_google_, options));
  EXPECT_TRUE(this->SetCookieWithOptions(
      cs.get(), this->url_google_, "B=A; httponly", options));
  this->MatchCookieLines(
      "A=C; B=A",
      this->GetCookiesWithOptions(cs.get(), this->url_google_, options));
  this->MatchCookieLines("A=C", this->GetCookies(cs.get(), this->url_google_));
}

TYPED_TEST_P(CookieStoreTest, TestCookieDeletion) {
  scoped_refptr<CookieStore> cs(this->GetCookieStore());

  
  EXPECT_TRUE(this->SetCookie(cs.get(), this->url_google_, kValidCookieLine));
  this->MatchCookieLines("A=B", this->GetCookies(cs.get(), this->url_google_));
  
  EXPECT_TRUE(this->SetCookie(cs.get(),
                              this->url_google_,
                              std::string(kValidCookieLine) + "; max-age=0"));
  this->MatchCookieLineWithTimeout(cs.get(), this->url_google_, std::string());

  
  EXPECT_TRUE(this->SetCookie(cs.get(), this->url_google_, kValidCookieLine));
  this->MatchCookieLines("A=B", this->GetCookies(cs.get(), this->url_google_));
  
  EXPECT_TRUE(this->SetCookie(cs.get(),
                              this->url_google_,
                              std::string(kValidCookieLine) +
                                  "; expires=Mon, 18-Apr-1977 22:50:13 GMT"));
  this->MatchCookieLines(std::string(),
                         this->GetCookies(cs.get(), this->url_google_));

  
  EXPECT_TRUE(this->SetCookie(
      cs.get(),
      this->url_google_,
      std::string(kValidCookieLine) + "; expires=Mon, 18-Apr-22 22:50:13 GMT"));

  this->MatchCookieLines("A=B", this->GetCookies(cs.get(), this->url_google_));
  
  EXPECT_TRUE(this->SetCookie(cs.get(),
                              this->url_google_,
                              std::string(kValidCookieLine) + "; max-age=0"));
  this->MatchCookieLineWithTimeout(cs.get(), this->url_google_, std::string());

  
  EXPECT_TRUE(this->SetCookie(
      cs.get(),
      this->url_google_,
      std::string(kValidCookieLine) + "; expires=Mon, 18-Apr-22 22:50:13 GMT"));
  this->MatchCookieLines("A=B", this->GetCookies(cs.get(), this->url_google_));
  
  EXPECT_TRUE(this->SetCookie(cs.get(),
                              this->url_google_,
                              std::string(kValidCookieLine) +
                                  "; expires=Mon, 18-Apr-1977 22:50:13 GMT"));
  this->MatchCookieLines(std::string(),
                         this->GetCookies(cs.get(), this->url_google_));

  
  EXPECT_TRUE(this->SetCookie(
      cs.get(),
      this->url_google_,
      std::string(kValidCookieLine) + "; expires=Mon, 18-Apr-22 22:50:13 GMT"));
  this->MatchCookieLines("A=B", this->GetCookies(cs.get(), this->url_google_));
  
  base::Time server_time;
  EXPECT_TRUE(base::Time::FromString("Sun, 17-Apr-1977 22:50:13 GMT",
                                     &server_time));
  EXPECT_TRUE(this->SetCookieWithServerTime(
      cs.get(),
      this->url_google_,
      std::string(kValidCookieLine) + "; expires=Mon, 18-Apr-1977 22:50:13 GMT",
      server_time));
  this->MatchCookieLines("A=B", this->GetCookies(cs.get(), this->url_google_));

  
  EXPECT_TRUE(this->SetCookie(
      cs.get(),
      this->url_google_,
      std::string(kValidCookieLine) + "; expires=Mon, 18-Apr-22 22:50:13 GMT"));
  this->MatchCookieLines("A=B", this->GetCookies(cs.get(), this->url_google_));
  
  EXPECT_TRUE(this->SetCookie(cs.get(),
                              this->url_google_,
                              std::string(kValidCookieLine) +
                                  "; expires=Thu, 1-Jan-1970 00:00:00 GMT"));
  this->MatchCookieLines(std::string(),
                         this->GetCookies(cs.get(), this->url_google_));
}

TYPED_TEST_P(CookieStoreTest, TestDeleteAllCreatedBetween) {
  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  const base::Time last_month = base::Time::Now() -
                                base::TimeDelta::FromDays(30);
  const base::Time last_minute = base::Time::Now() -
                                 base::TimeDelta::FromMinutes(1);
  const base::Time next_minute = base::Time::Now() +
                                 base::TimeDelta::FromMinutes(1);
  const base::Time next_month = base::Time::Now() +
                                base::TimeDelta::FromDays(30);

  
  EXPECT_TRUE(this->SetCookie(cs.get(), this->url_google_, "A=B"));
  
  this->MatchCookieLines("A=B", this->GetCookies(cs.get(), this->url_google_));

  
  EXPECT_EQ(0, this->DeleteCreatedBetween(cs.get(), last_month, last_minute));
  EXPECT_EQ(0, this->DeleteCreatedBetween(cs.get(), next_minute, next_month));
  
  this->MatchCookieLines("A=B", this->GetCookies(cs.get(), this->url_google_));

  
  EXPECT_EQ(1, this->DeleteCreatedBetween(cs.get(), last_minute, next_minute));
  
  this->MatchCookieLines(std::string(),
                         this->GetCookies(cs.get(), this->url_google_));

  
  EXPECT_TRUE(this->SetCookie(cs.get(), this->url_google_, "C=D"));
  
  this->MatchCookieLines("C=D", this->GetCookies(cs.get(), this->url_google_));

  
  EXPECT_EQ(1, this->DeleteCreatedBetween(cs.get(), last_minute, base::Time()));
  
  this->MatchCookieLines(std::string(),
                         this->GetCookies(cs.get(), this->url_google_));
}

TYPED_TEST_P(CookieStoreTest, TestSecure) {
    scoped_refptr<CookieStore> cs(this->GetCookieStore());

    EXPECT_TRUE(this->SetCookie(cs.get(), this->url_google_, "A=B"));
    this->MatchCookieLines("A=B",
                           this->GetCookies(cs.get(), this->url_google_));
    this->MatchCookieLines(
        "A=B", this->GetCookies(cs.get(), this->url_google_secure_));

  EXPECT_TRUE(
      this->SetCookie(cs.get(), this->url_google_secure_, "A=B; secure"));
  
  this->MatchCookieLines(std::string(),
                         this->GetCookies(cs.get(), this->url_google_));
  this->MatchCookieLines("A=B",
                         this->GetCookies(cs.get(), this->url_google_secure_));

  EXPECT_TRUE(
      this->SetCookie(cs.get(), this->url_google_secure_, "D=E; secure"));
  this->MatchCookieLines(std::string(),
                         this->GetCookies(cs.get(), this->url_google_));
  this->MatchCookieLines("A=B; D=E",
                         this->GetCookies(cs.get(), this->url_google_secure_));

  EXPECT_TRUE(this->SetCookie(cs.get(), this->url_google_secure_, "A=B"));
  
  this->MatchCookieLines("A=B", this->GetCookies(cs.get(), this->url_google_));
  this->MatchCookieLines("D=E; A=B",
                         this->GetCookies(cs.get(), this->url_google_secure_));
}

static const int kLastAccessThresholdMilliseconds = 200;

TYPED_TEST_P(CookieStoreTest, NetUtilCookieTest) {
  const GURL test_url("http://mojo.jojo.google.izzle/");

  scoped_refptr<CookieStore> cs(this->GetCookieStore());

  EXPECT_TRUE(this->SetCookie(cs.get(), test_url, "foo=bar"));
  std::string value = this->GetCookies(cs.get(), test_url);
  this->MatchCookieLines("foo=bar", value);

  
  EXPECT_TRUE(this->SetCookie(cs.get(), test_url, "x=1"));
  EXPECT_TRUE(this->SetCookie(cs.get(), test_url, "y=2"));

  std::string result = this->GetCookies(cs.get(), test_url);
  EXPECT_FALSE(result.empty());
  EXPECT_NE(result.find("x=1"), std::string::npos) << result;
  EXPECT_NE(result.find("y=2"), std::string::npos) << result;
}

TYPED_TEST_P(CookieStoreTest, OverwritePersistentCookie) {
  GURL url_google("http://www.google.com/");
  GURL url_chromium("http://chromium.org");
  scoped_refptr<CookieStore> cs(this->GetCookieStore());

  
  EXPECT_TRUE(this->SetCookie(cs.get(),
                              url_google,
                              "a=val1; path=/path1; "
                              "expires=Mon, 18-Apr-22 22:50:13 GMT"));

  
  EXPECT_TRUE(this->SetCookie(cs.get(),
                              url_google,
                              "b=val1; path=/path1; "
                              "expires=Mon, 18-Apr-22 22:50:14 GMT"));

  
  
  CookieOptions allow_httponly;
  allow_httponly.set_include_httponly();
  EXPECT_TRUE(this->SetCookieWithOptions(cs.get(),
                                         url_google,
                                         "b=val2; path=/path1; httponly; "
                                         "expires=Mon, 18-Apr-22 22:50:14 GMT",
                                         allow_httponly));

  
  EXPECT_TRUE(this->SetCookie(cs.get(),
                              url_google,
                              "a=val33; path=/path1; "
                              "expires=Mon, 18-Apr-22 22:50:14 GMT"));

  
  
  EXPECT_TRUE(this->SetCookie(cs.get(),
                              url_google,
                              "a=val9; path=/path2; "
                              "expires=Mon, 18-Apr-22 22:50:14 GMT"));

  
  
  
  EXPECT_TRUE(this->SetCookie(cs.get(),
                              url_chromium,
                              "a=val99; path=/path1; "
                              "expires=Mon, 18-Apr-22 22:50:14 GMT"));

  if (TypeParam::supports_http_only) {
    this->MatchCookieLines(
        "a=val33",
        this->GetCookies(cs.get(), GURL("http://www.google.com/path1")));
  } else {
    this->MatchCookieLines(
        "a=val33; b=val2",
        this->GetCookies(cs.get(), GURL("http://www.google.com/path1")));
  }
  this->MatchCookieLines(
      "a=val9",
      this->GetCookies(cs.get(), GURL("http://www.google.com/path2")));
  this->MatchCookieLines(
      "a=val99", this->GetCookies(cs.get(), GURL("http://chromium.org/path1")));
}

TYPED_TEST_P(CookieStoreTest, CookieOrdering) {
  
  
  
  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  EXPECT_TRUE(this->SetCookie(
      cs.get(), GURL("http://d.c.b.a.google.com/aa/x.html"), "c=1"));
  EXPECT_TRUE(this->SetCookie(cs.get(),
                              GURL("http://b.a.google.com/aa/bb/cc/x.html"),
                              "d=1; domain=b.a.google.com"));
  base::PlatformThread::Sleep(base::TimeDelta::FromMilliseconds(
      TypeParam::creation_time_granularity_in_ms));
  EXPECT_TRUE(this->SetCookie(cs.get(),
                              GURL("http://b.a.google.com/aa/bb/cc/x.html"),
                              "a=4; domain=b.a.google.com"));
  base::PlatformThread::Sleep(base::TimeDelta::FromMilliseconds(
      TypeParam::creation_time_granularity_in_ms));
  EXPECT_TRUE(this->SetCookie(cs.get(),
                              GURL("http://c.b.a.google.com/aa/bb/cc/x.html"),
                              "e=1; domain=c.b.a.google.com"));
  EXPECT_TRUE(this->SetCookie(
      cs.get(), GURL("http://d.c.b.a.google.com/aa/bb/x.html"), "b=1"));
  EXPECT_TRUE(this->SetCookie(
      cs.get(), GURL("http://news.bbc.co.uk/midpath/x.html"), "g=10"));
  EXPECT_EQ("d=1; a=4; e=1; b=1; c=1",
            this->GetCookies(cs.get(),
                             GURL("http://d.c.b.a.google.com/aa/bb/cc/dd")));
}

TYPED_TEST_P(CookieStoreTest, DeleteSessionCookie) {
  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  
  EXPECT_TRUE(this->SetCookie(
      cs.get(), this->url_google_, std::string(kValidCookieLine)));
  EXPECT_TRUE(this->SetCookie(cs.get(),
                              this->url_google_,
                              "C=D; path=/; domain=google.izzle;"
                              "expires=Mon, 18-Apr-22 22:50:13 GMT"));
  this->MatchCookieLines("A=B; C=D",
                         this->GetCookies(cs.get(), this->url_google_));
  
  this->DeleteSessionCookies(cs.get());
  
  EXPECT_EQ("C=D", this->GetCookies(cs.get(), this->url_google_));
}

REGISTER_TYPED_TEST_CASE_P(CookieStoreTest,
                           TypeTest,
                           DomainTest,
                           DomainWithTrailingDotTest,
                           ValidSubdomainTest,
                           InvalidDomainTest,
                           DomainWithoutLeadingDotTest,
                           CaseInsensitiveDomainTest,
                           TestIpAddress,
                           TestNonDottedAndTLD,
                           TestHostEndsWithDot,
                           InvalidScheme,
                           InvalidScheme_Read,
                           PathTest,
                           EmptyExpires,
                           HttpOnlyTest,
                           TestCookieDeletion,
                           TestDeleteAllCreatedBetween,
                           TestSecure,
                           NetUtilCookieTest,
                           OverwritePersistentCookie,
                           CookieOrdering,
                           DeleteSessionCookie);

template<class CookieStoreTestTraits>
class MultiThreadedCookieStoreTest :
    public CookieStoreTest<CookieStoreTestTraits> {
 public:
  MultiThreadedCookieStoreTest() : other_thread_("CMTthread") {}

  
  

  void GetCookiesTask(CookieStore* cs,
                      const GURL& url,
                      StringResultCookieCallback* callback) {
    CookieOptions options;
    if (!CookieStoreTestTraits::supports_http_only)
      options.set_include_httponly();
    cs->GetCookiesWithOptionsAsync(
        url, options,
        base::Bind(&StringResultCookieCallback::Run,
                   base::Unretained(callback)));
  }

  void GetCookiesWithOptionsTask(CookieStore* cs,
                                 const GURL& url,
                                 const CookieOptions& options,
                                 StringResultCookieCallback* callback) {
    cs->GetCookiesWithOptionsAsync(
        url, options,
        base::Bind(&StringResultCookieCallback::Run,
                   base::Unretained(callback)));
  }

  void SetCookieWithOptionsTask(CookieStore* cs,
                                const GURL& url,
                                const std::string& cookie_line,
                                const CookieOptions& options,
                                BoolResultCookieCallback* callback) {
    cs->SetCookieWithOptionsAsync(
        url, cookie_line, options,
        base::Bind(&BoolResultCookieCallback::Run, base::Unretained(callback)));
  }

  void DeleteCookieTask(CookieStore* cs,
                        const GURL& url,
                        const std::string& cookie_name,
                        NoResultCookieCallback* callback) {
    cs->DeleteCookieAsync(
        url, cookie_name,
        base::Bind(&NoResultCookieCallback::Run, base::Unretained(callback)));
  }

    void DeleteSessionCookiesTask(CookieStore* cs,
                                  IntResultCookieCallback* callback) {
    cs->DeleteSessionCookiesAsync(
        base::Bind(&IntResultCookieCallback::Run, base::Unretained(callback)));
  }

 protected:
  void RunOnOtherThread(const base::Closure& task) {
    other_thread_.Start();
    other_thread_.message_loop()->PostTask(FROM_HERE, task);
    CookieStoreTest<CookieStoreTestTraits>::RunFor(kTimeout);
    other_thread_.Stop();
  }

  Thread other_thread_;
};

TYPED_TEST_CASE_P(MultiThreadedCookieStoreTest);

TYPED_TEST_P(MultiThreadedCookieStoreTest, ThreadCheckGetCookies) {
  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  EXPECT_TRUE(this->SetCookie(cs.get(), this->url_google_, "A=B"));
  this->MatchCookieLines("A=B", this->GetCookies(cs.get(), this->url_google_));
  StringResultCookieCallback callback(&this->other_thread_);
  base::Closure task = base::Bind(
      &net::MultiThreadedCookieStoreTest<TypeParam>::GetCookiesTask,
      base::Unretained(this),
      cs, this->url_google_, &callback);
  this->RunOnOtherThread(task);
  EXPECT_TRUE(callback.did_run());
  EXPECT_EQ("A=B", callback.result());
}

TYPED_TEST_P(MultiThreadedCookieStoreTest, ThreadCheckGetCookiesWithOptions) {
  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  CookieOptions options;
  if (!TypeParam::supports_http_only)
    options.set_include_httponly();
  EXPECT_TRUE(this->SetCookie(cs.get(), this->url_google_, "A=B"));
  this->MatchCookieLines(
      "A=B", this->GetCookiesWithOptions(cs.get(), this->url_google_, options));
  StringResultCookieCallback callback(&this->other_thread_);
  base::Closure task = base::Bind(
      &net::MultiThreadedCookieStoreTest<TypeParam>::GetCookiesWithOptionsTask,
      base::Unretained(this),
      cs, this->url_google_, options, &callback);
  this->RunOnOtherThread(task);
  EXPECT_TRUE(callback.did_run());
  EXPECT_EQ("A=B", callback.result());
}

TYPED_TEST_P(MultiThreadedCookieStoreTest, ThreadCheckSetCookieWithOptions) {
  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  CookieOptions options;
  if (!TypeParam::supports_http_only)
    options.set_include_httponly();
  EXPECT_TRUE(
      this->SetCookieWithOptions(cs.get(), this->url_google_, "A=B", options));
  BoolResultCookieCallback callback(&this->other_thread_);
  base::Closure task = base::Bind(
      &net::MultiThreadedCookieStoreTest<TypeParam>::SetCookieWithOptionsTask,
      base::Unretained(this),
      cs, this->url_google_, "A=B", options, &callback);
  this->RunOnOtherThread(task);
  EXPECT_TRUE(callback.did_run());
  EXPECT_TRUE(callback.result());
}

TYPED_TEST_P(MultiThreadedCookieStoreTest, ThreadCheckDeleteCookie) {
  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  CookieOptions options;
  if (!TypeParam::supports_http_only)
    options.set_include_httponly();
  EXPECT_TRUE(
      this->SetCookieWithOptions(cs.get(), this->url_google_, "A=B", options));
  this->DeleteCookie(cs.get(), this->url_google_, "A");
  EXPECT_TRUE(
      this->SetCookieWithOptions(cs.get(), this->url_google_, "A=B", options));
  NoResultCookieCallback callback(&this->other_thread_);
  base::Closure task = base::Bind(
      &net::MultiThreadedCookieStoreTest<TypeParam>::DeleteCookieTask,
      base::Unretained(this),
      cs, this->url_google_, "A", &callback);
  this->RunOnOtherThread(task);
  EXPECT_TRUE(callback.did_run());
}

TYPED_TEST_P(MultiThreadedCookieStoreTest, ThreadCheckDeleteSessionCookies) {
  scoped_refptr<CookieStore> cs(this->GetCookieStore());
  CookieOptions options;
  if (!TypeParam::supports_http_only)
    options.set_include_httponly();
  EXPECT_TRUE(
      this->SetCookieWithOptions(cs.get(), this->url_google_, "A=B", options));
  EXPECT_TRUE(
      this->SetCookieWithOptions(cs.get(),
                                 this->url_google_,
                                 "B=C; expires=Mon, 18-Apr-22 22:50:13 GMT",
                                 options));
  EXPECT_EQ(1, this->DeleteSessionCookies(cs.get()));
  EXPECT_EQ(0, this->DeleteSessionCookies(cs.get()));
  EXPECT_TRUE(
      this->SetCookieWithOptions(cs.get(), this->url_google_, "A=B", options));
  IntResultCookieCallback callback(&this->other_thread_);
  base::Closure task = base::Bind(
      &net::MultiThreadedCookieStoreTest<TypeParam>::DeleteSessionCookiesTask,
      base::Unretained(this),
      cs, &callback);
  this->RunOnOtherThread(task);
  EXPECT_TRUE(callback.did_run());
  EXPECT_EQ(1, callback.result());
}

REGISTER_TYPED_TEST_CASE_P(MultiThreadedCookieStoreTest,
                           ThreadCheckGetCookies,
                           ThreadCheckGetCookiesWithOptions,
                           ThreadCheckSetCookieWithOptions,
                           ThreadCheckDeleteCookie,
                           ThreadCheckDeleteSessionCookies);

}  

#endif  
