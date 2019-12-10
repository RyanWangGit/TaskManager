import { connect } from "react-redux";
import PerformanceTab from '../components/performance-tab';

const mapStateToProps = (state, ownProps) => {
  return {
    className: ownProps.className,
    cpuLoadHistory: state.cpu.loadHistory,
    cpuInfo: state.cpu.info,
    memoryLoadHistory: state.memory.loadHistory,
    memoryLoad: state.memory.load,
    memoryInfo: state.memory.info
  };
};

const mapDispatchToProps = (dispatch) => {
  return {}
};

const PerformanceTabContainer = connect(
  mapStateToProps,
  mapDispatchToProps
)(PerformanceTab);

export default PerformanceTabContainer;