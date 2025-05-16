import axios from 'axios';

class ApiService {
  static async getAllData() {
    try {
      const response = await axios.get('http://192.168.1.36:5000/get_all_data');
      return response.data;
    } catch (error) {
      throw new Error('Failed to get all data');
    }
  }

  static async getWorkerData(username) {
    try {
      const response = await axios.get(`http://192.168.1.36:5000/get_worker_data/${username}`);
      return response.data;
    } catch (error) {
      throw new Error('Failed to get worker data');
    }
  }

  static async getDataByTimestamp(start_time, end_time) {
    try {
      const response = await axios.get(`http://192.168.1.36:5000/get_data_by_timestamp/${start_time}/${end_time}`);
      return response.data;
    } catch (error) {
      throw new Error('Failed to get data by timestamp');
    }
  }

  static async getDataByWorkerAndTimestamps(worker_name, start_time, end_time) {
    try {
      const response = await axios.get(`http://192.168.1.36:5000/get_data_by_worker_and_timestamps/${worker_name}/${start_time}/${end_time}`);
      return response.data;
    } catch (error) {
      throw new Error('Failed to get data by worker and timestamps');
    }
  }

  static async getWorkersAtWork(datestamp) {
    try {
      const response = await axios.get(`http://192.168.1.36:5000/get_workers_at_work/${datestamp}`);
      return response.data;
    } catch (error) {
      throw new Error('Failed to get workers at work');
    }
  }

  static async countAppearancesByTimestamp(start_time, end_time) {
    try {
      const response = await axios.get(`http://192.168.1.36:5000/count_appearances_by_timestamp/${start_time}/${end_time}`);
      return response.data;
    } catch (error) {
      throw new Error('Failed to count appearances by timestamp');
    }
  }
}

export default ApiService;
