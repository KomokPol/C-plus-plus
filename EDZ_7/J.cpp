#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>

class DJMixer {
 public:
  DJMixer() : next_free_id_(1) {
  }
  struct SongInfo {
    int song_id;
    int songScore;
    bool operator<(const SongInfo &other) const {
      if (songScore != other.songScore) {
        return songScore < other.songScore;
      }
      return song_id > other.song_id;
    }
  };
  const SongInfo& RegisterVote(int song_id, int vote) {
    if (songs_.count(song_id) == 0) {
      CreateSong(song_id, vote);
      return songs_[song_id];
    }
    UpdateSongScore(song_id, songs_[song_id].songScore + vote);
    return songs_[song_id];
  }
  const SongInfo& GetSongInfo(int song_id) {
    if (songs_.count(song_id) == 0) {
      CreateSong(song_id, 0);
    }
    return songs_[song_id];
  }
  SongInfo GetNextSong() {
    if (top_songs_.empty()) {
      CreateSong();
      return {next_free_id_, 0};
    }
    SongInfo current_song = top_songs_.top();
    top_songs_.pop();
    while (current_song.songScore != songs_[current_song.song_id].songScore) {
      current_song = top_songs_.top();
      top_songs_.pop();
    }
    int score = current_song.songScore;
    if (score < 0) {
      CreateSong();
      return {next_free_id_, 0};
    }
    if (score == 0) {
      while (songs_.count(next_free_id_) != 0 && next_free_id_ < current_song.song_id) {
        ++next_free_id_;
      }
      if (next_free_id_ < current_song.song_id) {
        top_songs_.push({current_song.song_id, 0});
        current_song.song_id = next_free_id_;
        songs_[next_free_id_] = {next_free_id_, 0};
      }
    }
    UpdateSongScore(current_song.song_id, -1);
    return {current_song.song_id, score};
  }
 private:
  void CreateSong(int song_id, int score) {
    songs_[song_id] = {song_id, score};
    top_songs_.push({song_id, score});
  }
  void CreateSong() {
    while (songs_.count(next_free_id_) != 0) {
      ++next_free_id_;
    }
    CreateSong(next_free_id_, -1);
  }
  void UpdateSongScore(int song_id, int score) {
    songs_[song_id].songScore = score;
    top_songs_.push(songs_[song_id]);
  }
  std::map<int, SongInfo> songs_;
  std::priority_queue<SongInfo> top_songs_;
  int next_free_id_;
};
int main() {
  std::map<std::string, int> last_vote_time;
  DJMixer mixer;
  std::string command;
  while (getline(std::cin, command)) {
    std::stringstream ss(command);
    std::string cmd;
    ss >> cmd;
    if (cmd == "VOTE") {
      std::string ip;
      int song_id = 0;
      int score = 0;
      int time = 0;
      ss >> ip >> song_id >> score >> time;
      if (last_vote_time.count(ip) == 0 || time - last_vote_time[ip] >= 600) {
        last_vote_time[ip] = time;
        mixer.RegisterVote(song_id, score);
      }
      std::cout << mixer.GetSongInfo(song_id).songScore << "\n";
    } else if (cmd == "GET") {
      DJMixer::SongInfo info = mixer.GetNextSong();
      std::cout << info.song_id << " " << info.songScore << "\n";
    } else if (cmd == "EXIT") {
      std::cout << "OK\n";
    }
  }
}