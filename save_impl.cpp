module save;

void save::prefetch(hai::fn<void> callback) {
  callback();
}
