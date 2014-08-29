#ifndef __SR_INSTANCE_H__
#define __SR_INSTANCE_H__

#include "settings.h"

namespace ZGen {

namespace ShiftReduce {

// Type define
typedef int                                       word_t;
typedef int                                       postag_t;
typedef int                                       deprel_t;
typedef std::vector<word_t>                       sentence_t;
typedef std::vector<std::pair<word_t, postag_t> > sentence_with_postag_t;


// The struct for dependency parse
struct DependencyParse {
  typedef std::pair<int, int> range_t;

  std::vector<word_t>     forms;
  // Storing the heads. For the pre-recognized NP, just the head is stored in
  // forms.

  std::vector<postag_t>   postags;
  // The postags.

  std::vector<int>        heads;
  //

  std::vector<deprel_t>   deprels;
  // The dependency relation.

  std::vector<word_t>     words;
  //

  std::vector<range_t>    phrases;
  //

  std::vector<bool>       is_phrases;

  std::vector<std::string> extras;

  void clear();

  void push_back(const word_t & form,
      const postag_t & postag,
      const int head,
      const deprel_t & deprel);

  void push_back(const word_t& form,
      const postag_t& postag,
      const int head,
      const deprel_t& deprel,
      const std::string& extra);

  void push_back(const word_t & form,
      const postag_t & postag,
      const int head,
      const deprel_t & deprel,
      const std::vector<word_t> & extended_words,
      const range_t & phrase,
      bool is_phrase);

  void push_back(const word_t& form,
      const postag_t& postag,
      const int head,
      const deprel_t& deprel,
      const std::vector<word_t>& extended_words,
      const range_t& phrase,
      bool is_phrase,
      const std::string& extra);

  size_t size() const;

  friend std::ostream & operator << (std::ostream & os, const DependencyParse & d);
};


typedef DependencyParse   dependency_t;
typedef DependencyParse   unordered_dependency_t;


}

}

#endif  //  end for __SR_INSTANCE_H__
